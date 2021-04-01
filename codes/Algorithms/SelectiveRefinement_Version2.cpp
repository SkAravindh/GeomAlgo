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

class SelectiveRefinement_2 {
public:
    SelectiveRefinement_2() = default;
    explicit SelectiveRefinement_2(std::shared_ptr<Mesh> &pmesh);
    void loadParameters(int refineL, double edgelen,bool enableoutchild=false);
    void run();
    void split(const EdgeOrder &ed);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(const EdgeOrder &currLongEdge,  Triangle* currenT, const EdgeOrder &NeighLongest_edge,Triangle *Neigh, bool flag,bool mid);
    void splitIntoThree(const EdgeOrder &currLongEdge,  Triangle* currenT, const EdgeOrder &NeighLongest_edge, Triangle *Neigh,bool flag,bool mid);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void checkNormalOrientation(Triangle* t);
    void flipNormal(Triangle* t);
    void defineSpliTyp(const EdgeOrder &ED, std::vector<Triangle*> &sharing_tri);


private:
    std::shared_ptr<Mesh> pMesh;

private:
    void doRefine();
    //Required Container
   // std::set<Triangle*> inputTriangles;
    std::set<EdgeOrder> edgesToRefine;
    //std::set<EdgeOrder> allEdges;
    std::set<EdgeOrder> visited_edges;
    std::set<EdgeOrder> border_Edges;
    std::set<EdgeOrder> nonManifold_Edges;
    std::set<EdgeOrder> visited_border_Edges;

    //track inisde and outside edges
    std::set<EdgeOrder> childEdgesInside;
    std::set<EdgeOrder> childEdgesOutside;

    //Iterators
    typedef std::set<Triangle*>::iterator ST_Itr;
    typedef std::set<EdgeOrder>::iterator SE_Itr;
    typedef std::vector<Triangle*>::iterator VT_itr;
    typedef std::vector<EdgeOrder>::iterator VE_itr;

    int refineLevel=1;
    double edgeLength = 1;
    bool enablechildoutside = false;
    bool hasmanifold = false;
};

SelectiveRefinement_2::SelectiveRefinement_2(std::shared_ptr<Mesh> &pmesh):pMesh(pmesh) {
}

void SelectiveRefinement_2::loadParameters(int refineL, double edgelen, bool enableoutchild) {

    refineLevel        = refineL;
    edgeLength         = edgelen;
    enablechildoutside = enableoutchild;

}

void SelectiveRefinement_2::run() {

    std::vector<EdgeOrder> b_edges;
    std::vector<EdgeOrder> nonmani_edges;
    pMesh->getBorder_Nonmanifold_Edges(b_edges,nonmani_edges);
    _Copy(b_edges.begin(),b_edges.end(),border_Edges);
    _Copy(nonmani_edges.begin(),nonmani_edges.end(),nonManifold_Edges);
    if(!nonManifold_Edges.empty()) {
        hasmanifold = true;
    }
    doRefine();
}

void SelectiveRefinement_2::doRefine() {

    std::vector<Triangle*> allTriangles;
    pMesh->getTriangles(allTriangles);

   // std::vector<Triangle*> Ring;
   // pMesh->getNeigTrianglesbyOrder(allTriangles[0],3,Ring);

    std::vector<EdgeOrder> EIT;

    getEdgesofTrianlges(allTriangles,EIT);


    for(VE_itr it = EIT.begin(); it != EIT.end(); it++) {
        edgesToRefine.insert(*it);
    }
    if(enablechildoutside) {
        std::cout << " ********** Edges outside of the patch have been included to refine ********** " << std::endl;
    }
    for(int i=0; i<refineLevel; i++) {

        std::cout<< "Iteration: " <<i <<" "<<"Edges_to_split "<<edgesToRefine.size()<< std::endl;
        for(SE_Itr itr = edgesToRefine.begin(); itr != edgesToRefine.end(); itr++) {
           // bool re = avoidDupicate(allEdges,*itr);
           // if(re) continue;
            SE_Itr it = visited_edges.find(*itr);
            if(it != visited_edges.end()) continue;
            if(border_Edges.find(*itr) != border_Edges.end()) continue;
            double current_edge_length = itr->getlength();
            if (current_edge_length > (4.0 / 3.6) *edgeLength ) {
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

void SelectiveRefinement_2::split(const EdgeOrder &ed) {

    std::vector<Triangle*> sharing_triangle;
    pMesh->getAdjustenNeigh_1(ed,sharing_triangle);

    if (sharing_triangle.size() != 2) {
        if(sharing_triangle.size() == 1 && isExist(border_Edges,ed)) {
            visited_border_Edges.insert(ed);
            defineSpliTyp(ed,sharing_triangle);
            return;
        } else{
            return;
        }
    }

    for (auto triangle : sharing_triangle) {

        Triangle* current_neigh_triangle(triangle);
        int id = current_neigh_triangle->getLongestEdgeID();
        EdgeOrder current_neigh_triangle_longED = current_neigh_triangle->getEO(id);
        HasCommonLE re = hassameedges(ed,current_neigh_triangle_longED);

        if (re == HasCommonLE::same) {
            splitIntoTwo(ed, nullptr,current_neigh_triangle_longED,current_neigh_triangle,true,true);
        }
        else if (re == HasCommonLE::not_same) {
            if (isExist(border_Edges, current_neigh_triangle_longED)) {
                splitIntoTwo(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, false,true); // mid--> fa to true
            } else {
                splitIntoThree(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, true,true);
            }
        }
    }
}

void SelectiveRefinement_2::defineSpliTyp(const EdgeOrder &ed, std::vector<Triangle*> &sharing_tri) {

    for(Triangle* ele : sharing_tri) {
        Triangle* current_neigh_triangle(ele);
        int id = current_neigh_triangle->getLongestEdgeID();
        EdgeOrder current_neigh_triangle_longED = current_neigh_triangle->getEO(id);
        HasCommonLE re = hassameedges(ed,current_neigh_triangle_longED);
        if (re == HasCommonLE::same) {
            splitIntoTwo(ed, nullptr,current_neigh_triangle_longED,current_neigh_triangle,true, false);
        } else if (re == HasCommonLE::not_same) {

            if (isExist(border_Edges, current_neigh_triangle_longED)) {
                splitIntoTwo(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, false,false); //chaged true to false---> flag
            }
            else {
                splitIntoThree(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, true,false);
            }
        }
    }
}

void SelectiveRefinement_2::splitIntoTwo(const EdgeOrder &currLongEdge, Triangle *currenT, const EdgeOrder &NeighLongest_edge, Triangle *Neigh, bool flag,bool mid) {

    Point* midpoint = currLongEdge.getMidPoint();
    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();

    if(!flag) {
        NeighT_LedgeID = Neigh->getPeakVertexID(currLongEdge);
    }

    Point* NeighT_peakvertex    =  Neigh->getCorners(NeighT_LedgeID);

    Point* RightVertex = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* LeftVertex = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(midpoint,NeighT_peakvertex,RightVertex,Neigh);
    pMesh->createTriangle(NeighT_peakvertex,midpoint,LeftVertex,Neigh);
    checkNormalOrientation(Neigh);
    EdgeOrder ed_1(midpoint,LeftVertex);
    EdgeOrder ed_2(midpoint,RightVertex);
    EdgeOrder ed_3(midpoint,NeighT_peakvertex);
    EdgeOrder ed_4(LeftVertex,NeighT_peakvertex);
    EdgeOrder ed_5(RightVertex,NeighT_peakvertex);

    if(!mid) {
        border_Edges.insert(EdgeOrder(midpoint, currLongEdge.p0));
        border_Edges.insert(EdgeOrder(midpoint, currLongEdge.p1));
    }

    if (isExist(edgesToRefine,NeighLongest_edge)) {
        visited_edges.insert(NeighLongest_edge);
        childEdgesInside.insert(ed_1);
        childEdgesInside.insert(ed_2);
        childEdgesInside.insert(ed_3);
        //childEdgesInside.insert(ed_4);
        //childEdgesInside.insert(ed_5);
    }
    else if (isExist(childEdgesInside,NeighLongest_edge )) {
        childEdgesInside.erase(NeighLongest_edge);
        childEdgesInside.insert(ed_1);
        childEdgesInside.insert(ed_2);
        childEdgesInside.insert(ed_3);
        //childEdgesInside.insert(ed_4);
        //childEdgesInside.insert(ed_5);
    }
    else if (enablechildoutside) {
      //  if (isExist(childEdgesOutside,NeighLongest_edge )) {
            childEdgesOutside.erase(NeighLongest_edge);
            childEdgesOutside.insert(ed_1);
            childEdgesOutside.insert(ed_2);
            childEdgesOutside.insert(ed_3);
            //childEdgesOutside.insert(ed_4);
            //childEdgesOutside.insert(ed_5);
       // }
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    deleteInfoFromDS(Neigh);
}

void SelectiveRefinement_2::splitIntoThree(const EdgeOrder &currLongEdge,  Triangle* currenT, const EdgeOrder &NeighLongest_edge, Triangle *Neigh,bool flag,bool mid) {

    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    std::vector<Triangle *> adjacent_triangles;
    pMesh->getAdjustenNeigh_1(NeighLongest_edge, adjacent_triangles);
    int adsize = adjacent_triangles.size();
    if (adsize == 0) {
        return;
    } else if (adsize > 2 || adsize==1)  {
        splitIntoTwo(currLongEdge, currenT, NeighLongest_edge, Neigh, false, true);
        return;
    }else{
        for(auto ele : adjacent_triangles) {
            if (*ele == *Neigh ) continue;
            second_direct_NeighT=ele;
        }
        int nT_LedgeID     = second_direct_NeighT->getLongestEdgeID();
        EdgeOrder nT_Ledge = second_direct_NeighT->getEO(nT_LedgeID);
        second_NeighT_Ledge         = nT_Ledge;
    }



    //getNeighTriandLedge(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge);

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
    checkNormalOrientation(Neigh);

    EdgeOrder c_ed1(CurrentT_LedgeMP,NeighT_peakVertex);
    EdgeOrder c_ed2(CurrentT_LedgeMP,RightVertex);
    EdgeOrder c_ed3(CurrentT_LedgeMP,NeighT_LedgeMP);

    EdgeOrder n_ed1(NeighT_LedgeMP,LeftVertex);
    EdgeOrder n_ed2(NeighT_LedgeMP,RightVertex);
    EdgeOrder n_ed3(NeighT_LedgeMP,NeighT_peakVertex);
    //EdgeOrder n_ed4(LeftVertex,NeighT_peakVertex);

    if(!mid) {
        border_Edges.insert(EdgeOrder(CurrentT_LedgeMP, currLongEdge.p0));
        border_Edges.insert(EdgeOrder(CurrentT_LedgeMP, currLongEdge.p1));
    }

    if (isExist(edgesToRefine,NeighLongest_edge)) {
        visited_edges.insert(NeighLongest_edge);
        childEdgesInside.insert(n_ed1);
        childEdgesInside.insert(n_ed2);
        childEdgesInside.insert(n_ed3);
       // childEdgesInside.insert(n_ed4);
    }
    else if(isExist(childEdgesInside,NeighLongest_edge)) {
        childEdgesInside.erase(NeighLongest_edge);
        childEdgesInside.insert(n_ed1);
        childEdgesInside.insert(n_ed2);
        childEdgesInside.insert(n_ed3);
       // childEdgesInside.insert(n_ed4);
    }
    else if (enablechildoutside) {
      //  if(isExist(childEdgesOutside,NeighLongest_edge)) {
            childEdgesOutside.erase(NeighLongest_edge);
            childEdgesOutside.insert(n_ed1);
            childEdgesOutside.insert(n_ed2);
            childEdgesOutside.insert(n_ed3);
            //childEdgesOutside.insert(n_ed4);
       // }
    }

    if (isExist(edgesToRefine,currLongEdge)) {
        visited_edges.insert(currLongEdge);
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

//    Triangle* second_direct_NeighT ;
//    EdgeOrder second_NeighT_Ledge;
//
//    getNeighTriandLedge(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge);

    HasCommonLE re = hassameedges(NeighLongest_edge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLongest_edge,Neigh,second_NeighT_Ledge,second_direct_NeighT ,true, true);
    }
    else if (re == HasCommonLE::not_same) {
        if (isExist(border_Edges, second_NeighT_Ledge)) {
            splitIntoTwo(NeighLongest_edge, Neigh, second_NeighT_Ledge, second_direct_NeighT, false, true); //chaged false to true---> flag
        } else {
            splitIntoThree(NeighLongest_edge, Neigh, second_NeighT_Ledge, second_direct_NeighT, true, true);
        }
    }

}

HasCommonLE SelectiveRefinement_2::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void SelectiveRefinement_2::checkNormalOrientation(Triangle *t) {

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

void  SelectiveRefinement_2::flipNormal(Triangle *t) {

    Point* vertex0 = t->getCorners(0);
    Point* vertex1 = t->getCorners(1);
    t->setNewVertex(vertex1,0);
    t->setNewVertex(vertex0,1);
}

void SelectiveRefinement_2::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);

    upDateDS(T);
}

void SelectiveRefinement_2::upDateDS(Triangle *T) {

    T->isAlive = false;
    std::vector<Triangle*> TV;
    T->getChildren(TV);
    pMesh->fillTriangleContainers(TV,alltri);

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

