#include "../Mesh.h"

enum class HasCommonLE {
    same,
    not_same
};

template<class T_key>
bool avoidDupicate(std::set<T_key> &container, const T_key &data);

template<class T_key>
bool isExist(std::set<T_key> & container, const T_key &data);

template<class Iter, class T>
void _Copy(Iter _begin, Iter _end, std::set<T> &container);

class DesiredEdge2_1Send {
public:
    DesiredEdge2_1Send() = default;
    explicit DesiredEdge2_1Send(std::shared_ptr<Mesh> &pmesh);
    void doRefine(int a);
    void split(const EdgeOrder &ed);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(const EdgeOrder &currLongEdge,  Triangle* currenT, const EdgeOrder &NeighLongest_edge,Triangle *Neigh, bool flag);
    void splitIntoThree(const EdgeOrder &currLongEdge,  Triangle* currenT, const EdgeOrder &NeighLongest_edge, Triangle *Neigh,bool flag);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge);
    void checkNormal(Triangle* t);
    void flipNormal(Triangle* t);
    bool checkForBorder(std::vector<EdgeOrder> & EV);

private:
    std::shared_ptr<Mesh> pMesh;

private:
    //Required Container
    std::set<Triangle*> inputTriangles;
    std::set<EdgeOrder> edgesToRefine;
    std::set<EdgeOrder> allEdges;
    std::set<EdgeOrder> visited;

    //track edges
    std::set<EdgeOrder> childEdgesInside;
    std::set<EdgeOrder> childEdgesOutside;
    bool enablechildoutside = false;
    //Iterators
    typedef std::set<Triangle*>::iterator ST_Itr;
    typedef std::set<EdgeOrder>::iterator SE_Itr;
    typedef std::vector<Triangle*>::iterator VT_itr;
    typedef std::vector<EdgeOrder>::iterator VE_itr;
};

DesiredEdge2_1Send::DesiredEdge2_1Send(std::shared_ptr<Mesh> &pmesh):pMesh(pmesh) {
}

void DesiredEdge2_1Send::doRefine(int a) {

    std::vector<Triangle*> allTriangles;
    pMesh->getTriangles(allTriangles);

    std::vector<Triangle*> Ring;
    pMesh->getNeigTrianglesbyOrder(allTriangles[a],3,Ring);

    std::vector<EdgeOrder> EIT;

    getEdgesofTrianlges(Ring,EIT);
    if(checkForBorder(EIT)) {
        std::cout<< "Input patch has non manifold properties: Process Terminated " << std::endl;
        return;
    }
    for(VE_itr it = EIT.begin(); it != EIT.end(); it++) {
        edgesToRefine.insert(*it);
    }
    if(enablechildoutside) {
        std::cout << "Outside edges has been enabled " << std::endl;
    }
    for(int i=0; i<3; i++) {
        std::cout<< "Iteration: " <<i <<" "<<"Edges_to_split "<<edgesToRefine.size()<< std::endl;
        for(SE_Itr itr = edgesToRefine.begin(); itr != edgesToRefine.end(); itr++) {
            bool re = avoidDupicate(allEdges,*itr);
            if(re) continue;
            SE_Itr it = visited.find(*itr);
            if(it != visited.end()) continue;
            double current_edge_length = itr->getlength();
            if (current_edge_length > (4.0 / 3.6) *0.5 ) {
                split(*itr);
            }
        }

        edgesToRefine.clear();
        _Copy(childEdgesInside.begin(),childEdgesInside.end(),edgesToRefine);

        if(enablechildoutside) {
            _Copy(childEdgesOutside.begin(),childEdgesOutside.end(),edgesToRefine);
        }
        std::cout << "Edges_inside: " << childEdgesInside.size()<<" "<<"Edges_outside: " <<childEdgesOutside.size()<< std::endl;
        childEdgesInside.clear();
        childEdgesOutside.clear();
        std::cout << "edges_to_refine " << edgesToRefine.size() << std::endl;
        if (edgesToRefine.empty()) {
            std::cout<< "Stop at iteration: " << i << " " << " Since it is reached given edge threshold " << std::endl;
            break;
        }
    }
}

void DesiredEdge2_1Send::split(const EdgeOrder &ed) {

    std::vector<Triangle*> sharing_triangle;
    pMesh->getAdjustenNeigh_1(ed,sharing_triangle);
    if (sharing_triangle.size() != 2) {
       // std::cout << "C_edge " <<ed<<std::endl;
        std::cout << "triangle vector size is not two at split function  " << std::endl;
        return;
    }
    for (auto triangle : sharing_triangle) {
        Triangle* current_neigh_triangle(triangle);
        int id = current_neigh_triangle->getLongestEdgeID();
        EdgeOrder current_neigh_triangle_longED = current_neigh_triangle->getEO(id);
        HasCommonLE re = hassameedges(ed,current_neigh_triangle_longED);
        if (re == HasCommonLE::same) {
            splitIntoTwo(ed, nullptr,current_neigh_triangle_longED,current_neigh_triangle,true);
        }
        else if (re == HasCommonLE::not_same) {
            splitIntoThree(ed, nullptr,current_neigh_triangle_longED,current_neigh_triangle,true);
        }
    }
}

void DesiredEdge2_1Send::splitIntoTwo(const EdgeOrder &currLongEdge, Triangle *currenT, const EdgeOrder &NeighLongest_edge, Triangle *Neigh, bool flag) {

    Point* midpoint = currLongEdge.getMidPoint();
    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();
    Point* NeighT_peakvertex    =  Neigh->getCorners(NeighT_LedgeID);

    Point* RightVertex = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* LeftVertex = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(midpoint,NeighT_peakvertex,RightVertex,Neigh);
    pMesh->createTriangle(NeighT_peakvertex,midpoint,LeftVertex,Neigh);
    checkNormal(Neigh);
    EdgeOrder ed_1(midpoint,LeftVertex);
    EdgeOrder ed_2(midpoint,RightVertex);
    EdgeOrder ed_3(midpoint,NeighT_peakvertex);

    if (isExist(edgesToRefine,NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        childEdgesInside.insert(ed_1);
        childEdgesInside.insert(ed_2);
        childEdgesInside.insert(ed_3);
    }
    else if (isExist(childEdgesInside,NeighLongest_edge )) {
        childEdgesInside.erase(NeighLongest_edge);
        childEdgesInside.insert(ed_1);
        childEdgesInside.insert(ed_2);
        childEdgesInside.insert(ed_3);
    }
    else if (enablechildoutside) {
      //  if (isExist(childEdgesOutside,NeighLongest_edge )) {
            childEdgesOutside.erase(NeighLongest_edge);
            childEdgesOutside.insert(ed_1);
            childEdgesOutside.insert(ed_2);
            childEdgesOutside.insert(ed_3);
       // }
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    deleteInfoFromDS(Neigh);
}

void DesiredEdge2_1Send::splitIntoThree(const EdgeOrder &currLongEdge,  Triangle* currenT, const EdgeOrder &NeighLongest_edge, Triangle *Neigh,bool flag) {

    Point* NeighT_LedgeMP     =  NeighLongest_edge.getMidPoint();
    Point* CurrentT_LedgeMP   =  currLongEdge.getMidPoint();
    int NeighT_LedgeID        =  Neigh->getLongestEdgeID();
    Point* NeighT_peakVertex  =  Neigh->getCorners(NeighT_LedgeID);

    Point* RightVertex = currLongEdge.p0;
    Point* LeftVertex = NeighLongest_edge.p1;

    if (*currLongEdge.p0 == *NeighT_peakVertex) {
        RightVertex = currLongEdge.p1;
    }

    if (*RightVertex == *NeighLongest_edge.p1) {
        LeftVertex = NeighLongest_edge.p0;
    }

    pMesh->createTriangle(NeighT_peakVertex,CurrentT_LedgeMP,NeighT_LedgeMP,Neigh);
    pMesh->createTriangle(NeighT_peakVertex,NeighT_LedgeMP,LeftVertex,Neigh);
    pMesh->createTriangle(RightVertex,NeighT_LedgeMP,CurrentT_LedgeMP, Neigh);
    checkNormal(Neigh);

    EdgeOrder c_ed1(CurrentT_LedgeMP,NeighT_peakVertex);
    EdgeOrder c_ed2(CurrentT_LedgeMP,RightVertex);
    EdgeOrder c_ed3(CurrentT_LedgeMP,NeighT_LedgeMP);

    EdgeOrder n_ed1(NeighT_LedgeMP,LeftVertex);
    EdgeOrder n_ed2(NeighT_LedgeMP,RightVertex);
    EdgeOrder n_ed3(NeighT_LedgeMP,NeighT_peakVertex);

    if (isExist(edgesToRefine,NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        childEdgesInside.insert(n_ed1);
        childEdgesInside.insert(n_ed2);
        childEdgesInside.insert(n_ed3);
    }
    else if(isExist(childEdgesInside,NeighLongest_edge)) {
        childEdgesInside.erase(NeighLongest_edge);
        childEdgesInside.insert(n_ed1);
        childEdgesInside.insert(n_ed2);
        childEdgesInside.insert(n_ed3);
    }
    else if (enablechildoutside) {
      //  if(isExist(childEdgesOutside,NeighLongest_edge)) {
            childEdgesOutside.erase(NeighLongest_edge);
            childEdgesOutside.insert(n_ed1);
            childEdgesOutside.insert(n_ed2);
            childEdgesOutside.insert(n_ed3);
       // }
    }

    if (isExist(edgesToRefine,currLongEdge)) {
        visited.insert(currLongEdge);
        childEdgesInside.insert(c_ed1);
        childEdgesInside.insert(c_ed2);
        childEdgesInside.insert(c_ed3);
    }
    else if(isExist(childEdgesInside,currLongEdge)) {
        childEdgesInside.erase(currLongEdge);
        childEdgesInside.insert(c_ed1);
        childEdgesInside.insert(c_ed2);
        childEdgesInside.insert(c_ed3);
    }
    else if (enablechildoutside) {
       // if(isExist(childEdgesOutside,currLongEdge)) {
            childEdgesOutside.erase(currLongEdge);
            childEdgesOutside.insert(c_ed1);
            childEdgesOutside.insert(c_ed2);
            childEdgesOutside.insert(c_ed3);
       // }
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }

    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge);

    HasCommonLE re = hassameedges(NeighLongest_edge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLongest_edge,Neigh,second_NeighT_Ledge,second_direct_NeighT ,true);
    }
    else if (re == HasCommonLE::not_same) {
        splitIntoThree(NeighLongest_edge, Neigh, second_NeighT_Ledge,second_direct_NeighT, true);
    }

}

void DesiredEdge2_1Send::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh_1(ed,adjacent_triangles);


    if ( adjacent_triangles.size() != 2) {
        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
        // std::cout << "ed " <<ed << std::endl;
        // std::cout << "ct " <<*CT << std::endl;
        return;
    }

    for(auto ele : adjacent_triangles) {
        if (*ele == *CT ) continue;
        Neigh=ele;
    }

    int nT_LedgeID     = Neigh->getLongestEdgeID();
    EdgeOrder nT_Ledge = Neigh->getEO(nT_LedgeID);
    NeighLedge         = nT_Ledge;

}

HasCommonLE DesiredEdge2_1Send::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void DesiredEdge2_1Send::checkNormal(Triangle *t) {
    std::vector<Triangle*> kids;
    t->getChildren(kids);
    Vector3 partentNormal = t->getNormalVector();
    for (auto ele : kids) {
        Vector3 childNorm = ele->getNormalVector();
        double re = dot(partentNormal,childNorm);
        if (re < 0) {
            flipNormal(ele);
        }
    }
}

void  DesiredEdge2_1Send::flipNormal(Triangle *t) {
    Point* vertex0 = t->getCorners(0);
    Point* vertex1 = t->getCorners(1);
    t->setNewVertex(vertex1,0);
    t->setNewVertex(vertex0,1);
}

void DesiredEdge2_1Send::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);

    upDateDS(T);
}

void DesiredEdge2_1Send::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);
    pMesh->fillTriangleContainers(TV,alltri);

}

bool DesiredEdge2_1Send::checkForBorder(std::vector<EdgeOrder> &EV) {
    bool hasmanifold = false;

    if (EV.empty()) {
        std::cout << "Input edge vector is empty " << std::endl;
        return true;
    }

    for (auto edge : EV) {
        std::vector<Triangle*> sharing_triangles;
        pMesh->getAdjustenNeigh(edge,sharing_triangles);
        int size = sharing_triangles.size();
        sharing_triangles.clear();
        if(size != 2) {
            hasmanifold = true;
            break;
        }
    }
    if( hasmanifold) {
        return true;
    } else {
        return false;
    }
 }

template<class T_key>
bool avoidDupicate(std::set<T_key> &container, const T_key &data) {

    typedef typename std::set<T_key>:: iterator itr;
    itr it = container.find(data);
    if( it != container.end()) {
        return true;
    }else {
        container.insert(data);
        return false;
    }
}

template<class T_key>
bool isExist(std::set<T_key> & container, const T_key &data) {

    typedef typename std::set<T_key>::iterator iter;
    iter it = container.find(data);
    if (it != container.end()) {
        return true;
    } else{
        return false;
    }
}

template<class Iter, class T>
void _Copy(Iter _begin, Iter _end, std::set<T> &container) {

    while(_begin != _end ) {
        container.insert(*_begin);
        _begin++;
    }
}

