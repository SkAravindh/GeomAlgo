#include "../Mesh.h"

enum class HasCommonLE {
    same,
    not_same
};

template<class T_key>
bool avoidDupicate(std::set<T_key> &container, const T_key &data);

bool isExist( std::set<EdgeOrder*,CompareEdgeOrder> &container, EdgeOrder* key);

template<class Iter, class T>
void _Copy(Iter _begin, Iter _end, std::set<T> &container);
void getEdgesofTrianlges_1( const std::vector<Triangle* > &TV, std::vector<EdgeOrder*> &BE);
class DesiredEdge2_1Send {
public:
    DesiredEdge2_1Send() = default;
    explicit DesiredEdge2_1Send(std::shared_ptr<Mesh> &pmesh);
    void doRefine(int a);
    void split(EdgeOrder* ed);
    HasCommonLE hassameedges(EdgeOrder* ed1,EdgeOrder* ed2);
    void splitIntoTwo(EdgeOrder* currLongEdge,  Triangle* currenT, EdgeOrder* NeighLongest_edge,Triangle *Neigh, bool flag, bool mid);
    void splitIntoThree(EdgeOrder* currLongEdge,  Triangle* currenT,EdgeOrder* NeighLongest_edge, Triangle *Neigh,bool flag,bool mid);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge);
    void checkNormal(Triangle* t);
    void flipNormal(Triangle* t);
    void markborder(Triangle* t, EdgeOrder* ed1, EdgeOrder* ed2, bool flag, std::vector<EdgeOrder*> outedges);
    bool checkForBorder(std::vector<EdgeOrder> & EV);

    void fun(EdgeOrder* ED, std::vector<Triangle*> &sharing_tri);

private:
    std::shared_ptr<Mesh> pMesh;

private:
    //Required Container
    std::set<Triangle*> inputTriangles;
    std::set<EdgeOrder*,CompareEdgeOrder> edgesToRefine;
    std::set<EdgeOrder> allEdges;
    std::set<EdgeOrder*,CompareEdgeOrder> visited;
   // std::set<EdgeOrder*,CompareEdgeOrder> border_Edges;
    std::set<EdgeOrder> nonManifold_Edges;
    std::set<EdgeOrder*,CompareEdgeOrder> visited_border_Edges;

    //track edges
    std::set<EdgeOrder*,CompareEdgeOrder> childEdgesInside;
    std::set<EdgeOrder*,CompareEdgeOrder> childEdgesOutside;
    bool enablechildoutside = false;
    //Iterators
    typedef std::set<Triangle*>::iterator ST_Itr;
    typedef std::set<EdgeOrder*>::iterator SE_Itr;
    typedef std::vector<Triangle*>::iterator VT_itr;
    typedef std::vector<EdgeOrder*>::iterator VE_itr;
};
void getEdgesofTrianlges_1( const std::vector<Triangle* > &TV, std::vector<EdgeOrder*> &BE) {

    std::set<EdgeOrder*,CompareEdgeOrder> edge;
    for(auto triangle : TV) {
        Triangle* CT(triangle);
        for (int i=0; i<3; i++) {
            EdgeOrder* ed = CT->getEO_pointer(i);
            edge.insert(ed);
        }
    }
    BE.assign(edge.begin(),edge.end());
}
DesiredEdge2_1Send::DesiredEdge2_1Send(std::shared_ptr<Mesh> &pmesh):pMesh(pmesh) {
}

void DesiredEdge2_1Send::doRefine(int a) {

    std::vector<Triangle*> allTriangles;
    pMesh->getTriangles(allTriangles);
    std::cout<<"allTriangles " <<allTriangles.size()<<std::endl;
    std::vector<EdgeOrder> b_edges;
    std::vector<EdgeOrder> nonmani_edges;

   // getedgesByOrder(allTriangles,-1,b_edges);
    pMesh->getBorder_Nonmanifold_Edges(b_edges,nonmani_edges);
  //  _Copy(b_edges.begin(),b_edges.end(),border_Edges);
    _Copy(nonmani_edges.begin(),nonmani_edges.end(),nonManifold_Edges);
//     for(auto ele : nonManifold_Edges ) {
//         auto iit = border_Edges.find(ele);
//         if (iit == border_Edges.end()) {
//             std::cout << "exisit " << std::endl;
//         }
//     }
   // std::vector<Point*> bpoints;
  //  getBorderPoints(b_edges,bpoints);
  //  writePoints("points.vtk", bpoints);
    std::vector<Triangle*> Ring;
   // writeSTL("input"+std::to_string(a)+".stl", Ring);
   pMesh->getNeigTrianglesbyOrder(allTriangles[a],2,Ring);
 //   writeSTL("input"+std::to_string(a)+".stl", Ring);
    std::vector<EdgeOrder*> EIT;

    getEdgesofTrianlges_1(Ring,EIT);
//    if(checkForBorder(EIT)) {
//        std::cout<< "Input patch has non manifold properties: Process Terminated " << std::endl;
//        return;
//    }
    for(VE_itr it = EIT.begin(); it != EIT.end(); it++) {
        edgesToRefine.insert(*it);
    }
    if(enablechildoutside) {
        std::cout << "Outside edges has been enabled " << std::endl;
    }
    for(int i=0; i<2; i++) {
        int count = 0;
        std::cout<< "Iteration: " <<i <<" "<<"Edges_to_split "<<edgesToRefine.size()<< std::endl;
        for(SE_Itr itr = edgesToRefine.begin(); itr != edgesToRefine.end(); itr++) {

           // bool re = avoidDupicate(allEdges,*itr);
            //if(re) continue;
            EdgeOrder* pointered(*itr);
            SE_Itr it = visited.find(pointered);
            if(it != visited.end()) continue;
        //    if(border_Edges.find(*itr) != border_Edges.end()) continue;
            double current_edge_length = pointered->getlength();
            if (current_edge_length > (4.0 / 3.6) *3) {

                split(pointered);
            }
        }

        edgesToRefine.clear();
        //_Copy(childEdgesInside.begin(),childEdgesInside.end(),edgesToRefine);
        for(auto ele : childEdgesInside) {
            edgesToRefine.insert(ele);
        }
        if(enablechildoutside) {
            //_Copy(childEdgesOutside.begin(),childEdgesOutside.end(),edgesToRefine);
            for(auto ele : childEdgesOutside) {
                edgesToRefine.insert(ele);
            }
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

void DesiredEdge2_1Send::split(EdgeOrder* ed) {

    std::vector<Triangle*> sharing_triangle;
    pMesh->getAdjustenNeigh_1(*ed,sharing_triangle);

    if (sharing_triangle.size() != 2) {
        if(sharing_triangle.size() == 1 && ed->status==EdgeStatus::Border) {
            visited_border_Edges.insert(ed);
            fun(ed,sharing_triangle);
            return;
        } else{
            return;
            std::cout << "triangle vector size is not either two or one at split function  " << std::endl;

        }
    }

    for (auto triangle : sharing_triangle) {
        Triangle* current_neigh_triangle(triangle);
        int id = current_neigh_triangle->getLongestEdgeID_1();
       
        EdgeOrder* current_neigh_triangle_longED = current_neigh_triangle->getEO_pointer(id);
       
        HasCommonLE re = hassameedges(ed,current_neigh_triangle_longED);
        if (re == HasCommonLE::same) {
           // std::cout << "first split into two " << std::endl;
            splitIntoTwo(ed, nullptr,current_neigh_triangle_longED,current_neigh_triangle,true,true);
        }
        else if (re == HasCommonLE::not_same) {

            if (current_neigh_triangle_longED->status==EdgeStatus::Border) {
                splitIntoTwo(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, false,true); // mid--> fa to true
            } else {
                splitIntoThree(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, true,true);
            }

        }


    }
}

void DesiredEdge2_1Send::fun(EdgeOrder* ed, std::vector<Triangle*> &sharing_tri) {

    for(Triangle* ele : sharing_tri) {
        Triangle* current_neigh_triangle(ele);
        int id = current_neigh_triangle->getLongestEdgeID_1();
        EdgeOrder* current_neigh_triangle_longED = current_neigh_triangle->getEO_pointer(id);
        HasCommonLE re = hassameedges(ed,current_neigh_triangle_longED);
        if (re == HasCommonLE::same) {
            splitIntoTwo(ed, nullptr,current_neigh_triangle_longED,current_neigh_triangle,true, false);
        } else if (re == HasCommonLE::not_same) {

            if ( current_neigh_triangle_longED->status==EdgeStatus::Border) {
               splitIntoTwo(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, false,false); //chaged true to false---> flag
           }
            else {
               splitIntoThree(ed, nullptr, current_neigh_triangle_longED, current_neigh_triangle, true,false);
           }

        }
    }
}
void DesiredEdge2_1Send::splitIntoTwo(EdgeOrder* currLongEdge, Triangle *currenT, EdgeOrder* NeighLongest_edge, Triangle *Neigh, bool flag,bool mid) {
  //  std::cout << "split into two " << std::endl;
    Point* midpoint = currLongEdge->getMidPoint();
    int NeighT_LedgeID          =  Neigh->getLongestEdgeID_1();

    if(!flag) {
        NeighT_LedgeID = Neigh->getPeakVertexID_1(currLongEdge);
    }
    Point* NeighT_peakvertex    =  Neigh->getCorners(NeighT_LedgeID);

    Point* RightVertex = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* LeftVertex = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(midpoint,NeighT_peakvertex,RightVertex,Neigh);
    pMesh->createTriangle(NeighT_peakvertex,midpoint,LeftVertex,Neigh);
    checkNormal(Neigh);
    EdgeOrder ed_1(midpoint,LeftVertex);
    EdgeOrder ed_2(midpoint,RightVertex);
    EdgeOrder ed_3(midpoint,NeighT_peakvertex);
    EdgeOrder ed_4(LeftVertex,NeighT_peakvertex);
    EdgeOrder ed_5(RightVertex,NeighT_peakvertex);
    std::vector<EdgeOrder*> outedges;
if(mid) {
    markborder(Neigh, nullptr, nullptr, false,outedges);
}

    if(!mid) {
      //  border_Edges.insert(EdgeOrder(midpoint, currLongEdge.p0));
        //border_Edges.insert(EdgeOrder(midpoint, currLongEdge.p1));
        EdgeOrder* marked1 = new EdgeOrder(midpoint, currLongEdge->p0);
        EdgeOrder* marked2 = new EdgeOrder(midpoint, currLongEdge->p1);
        markborder(Neigh,marked1,marked2, true,outedges);
    }

    if (isExist(edgesToRefine,NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        for(auto ele: outedges) {
            childEdgesInside.insert(ele);
        }
    }
    else if (isExist(childEdgesInside,NeighLongest_edge )) {
        childEdgesInside.erase(NeighLongest_edge);
        for(auto ele: outedges) {
            childEdgesInside.insert(ele);
        }
    }
    else if (enablechildoutside) {
      //  if (isExist(childEdgesOutside,NeighLongest_edge )) {
            childEdgesOutside.erase(NeighLongest_edge);
        for(auto ele: outedges) {
            childEdgesOutside.insert(ele);
        }
       // }
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    deleteInfoFromDS(Neigh);
}
void DesiredEdge2_1Send::markborder(Triangle* t, EdgeOrder* ed1, EdgeOrder* ed2, bool flag,std::vector<EdgeOrder*> outedges ) {

    std::vector<Triangle*> kids;
    std::set<EdgeOrder*,CompareEdgeOrder> edges;
    t->getChildren(kids);
    for(Triangle* tri:kids) {
        for(int i=0; i<3; i++){
           EdgeOrder temped = tri->getEO(i);
           EdgeOrder* poitered = new EdgeOrder(temped.p0,temped.p1);
            edges.insert(poitered);
           tri->setEdge(poitered,i);
           if(flag) {
               if (temped == *ed1 || temped == *ed2) {
                   poitered->status = EdgeStatus::Border;
               }
           }
        }
    }
    outedges.assign(edges.begin(),edges.end());
}

void DesiredEdge2_1Send::splitIntoThree(EdgeOrder* currLongEdge,  Triangle* currenT, EdgeOrder* NeighLongest_edge, Triangle *Neigh,bool flag,bool mid) {


    std::vector<Triangle*> adjacent_triangles;
    Triangle* second_direct_NeighT ;
    EdgeOrder* second_NeighT_Ledge;

    pMesh->getAdjustenNeigh_1(*NeighLongest_edge,adjacent_triangles);
    int adsize = adjacent_triangles.size();
    if(adsize == 0) {
        return;
    }
    else if(adsize >2 || adsize==1 ) {
   //     std::cout<< "greater two recent " << std::endl;
        splitIntoTwo(currLongEdge, currenT, NeighLongest_edge, Neigh, false,true);
        return;
    }else{
        for(auto ele : adjacent_triangles) {
            if (*ele == *Neigh ) continue;
            second_direct_NeighT=ele;
        }
        int nT_LedgeID     = second_direct_NeighT->getLongestEdgeID();
        EdgeOrder* nT_Ledge = second_direct_NeighT->getEO_pointer(nT_LedgeID);
        second_NeighT_Ledge         = nT_Ledge;
    }


    Point* NeighT_LedgeMP     =  NeighLongest_edge->getMidPoint();
    Point* CurrentT_LedgeMP   =  currLongEdge->getMidPoint();
    int NeighT_LedgeID        =  Neigh->getLongestEdgeID();
    Point* NeighT_peakVertex  =  Neigh->getCorners(NeighT_LedgeID);

    Point* RightVertex = currLongEdge->p0;
    Point* LeftVertex = NeighLongest_edge->p1;

    if (*currLongEdge->p0 == *NeighT_peakVertex) {
        RightVertex = currLongEdge->p1;
    }

    if (*RightVertex == *NeighLongest_edge->p1) {
        LeftVertex = NeighLongest_edge->p0;
    }

    pMesh->createTriangle(NeighT_peakVertex,CurrentT_LedgeMP,NeighT_LedgeMP,Neigh);
    pMesh->createTriangle(NeighT_peakVertex,NeighT_LedgeMP,LeftVertex,Neigh);
    pMesh->createTriangle(RightVertex,NeighT_LedgeMP,CurrentT_LedgeMP, Neigh);
    checkNormal(Neigh);
    std::map<EdgeOrder,int> child_insidemap;
    std::map<EdgeOrder,int> child_outsidemap;
    EdgeOrder c_ed1(CurrentT_LedgeMP,NeighT_peakVertex);
    EdgeOrder c_ed2(CurrentT_LedgeMP,RightVertex);
    EdgeOrder c_ed3(CurrentT_LedgeMP,NeighT_LedgeMP);

    EdgeOrder n_ed1(NeighT_LedgeMP,LeftVertex);
    EdgeOrder n_ed2(NeighT_LedgeMP,RightVertex);
    EdgeOrder n_ed3(NeighT_LedgeMP,NeighT_peakVertex);
    EdgeOrder n_ed4(LeftVertex,NeighT_peakVertex);
    child_insidemap.insert(std::make_pair(n_ed1,0));
    child_insidemap.insert(std::make_pair(n_ed2,1));
    child_insidemap.insert(std::make_pair(n_ed3,2));
    child_insidemap.insert(std::make_pair(n_ed4,3));

    child_outsidemap.insert(std::make_pair(c_ed1,0));
    child_outsidemap.insert(std::make_pair(c_ed2,1));
    child_outsidemap.insert(std::make_pair(c_ed3,2));
    std::vector<EdgeOrder*> outedges;
    if(mid) {
        markborder(Neigh, nullptr, nullptr, false,outedges);
    }



    if(!mid) {
        //border_Edges.insert(EdgeOrder(CurrentT_LedgeMP, currLongEdge->p0));
        //border_Edges.insert(EdgeOrder(CurrentT_LedgeMP, currLongEdge->p1));
        EdgeOrder* marked1 = new EdgeOrder(CurrentT_LedgeMP, currLongEdge->p0);
        EdgeOrder* marked2 = new EdgeOrder(CurrentT_LedgeMP, currLongEdge->p1);
        markborder(Neigh,marked1,marked2, true,outedges);
    }

    if (isExist(edgesToRefine,NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        for(auto ele : outedges) {
            if(child_insidemap.find(*ele) != child_insidemap.end()) {
                childEdgesInside.insert(ele);
            }
        }
    }
    else if(isExist(childEdgesInside,NeighLongest_edge)) {
        childEdgesInside.erase(NeighLongest_edge);
        for(auto ele : outedges) {
            if(child_insidemap.find(*ele) != child_insidemap.end()) {
                childEdgesInside.insert(ele);
            }
        }
    }
    else if (enablechildoutside) {
      //  if(isExist(childEdgesOutside,NeighLongest_edge)) {
            childEdgesOutside.erase(NeighLongest_edge);
        for(auto ele : outedges) {
            if(child_insidemap.find(*ele) != child_insidemap.end()) {
                childEdgesOutside.insert(ele);
            }
        }
       // }
    }

    if (isExist(edgesToRefine,currLongEdge)) {
        visited.insert(currLongEdge);
        for(auto ele : outedges) {
            if(child_outsidemap.find(*ele) != child_outsidemap.end()) {
                childEdgesInside.insert(ele);
            }
        }
    }
    else if(isExist(childEdgesInside,currLongEdge)) {
        childEdgesInside.erase(currLongEdge);
        for(auto ele : outedges) {
            if(child_outsidemap.find(*ele) != child_outsidemap.end()) {
                childEdgesInside.insert(ele);
            }
        }
    }
    else if (enablechildoutside) {
       // if(isExist(childEdgesOutside,currLongEdge)) {
        for(auto ele : outedges) {
            if(child_outsidemap.find(*ele) != child_outsidemap.end()) {
                childEdgesOutside.insert(ele);
            }
        }
       // }
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }


    HasCommonLE re = hassameedges(NeighLongest_edge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLongest_edge,Neigh,second_NeighT_Ledge,second_direct_NeighT ,true,true);
    }
    else if (re == HasCommonLE::not_same) {

         if ( second_NeighT_Ledge->status==EdgeStatus::Border) {
            splitIntoTwo(NeighLongest_edge, Neigh, second_NeighT_Ledge, second_direct_NeighT, false, true); //chaged false to true---> flag
        } else {
            splitIntoThree(NeighLongest_edge, Neigh, second_NeighT_Ledge, second_direct_NeighT, true, true);
        }

    }

}

/*void DesiredEdge2_1Send::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh_1(ed,adjacent_triangles);

    if (!( (adjacent_triangles.size() == 1 && isExist(border_Edges,ed)) || (adjacent_triangles.size() == 2) ) ) {
        std::cout<<"adjacent_triangles "<<adjacent_triangles.size()<< std::endl;
        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
        // std::cout << "ed " <<ed << std::endl;
        // std::cout << "ct " <<*CT << std::endl;
        return;
    }
//    if ( adjacent_triangles.size() != 2) {
//        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
//        // std::cout << "ed " <<ed << std::endl;
//        // std::cout << "ct " <<*CT << std::endl;
//        return;
//    }

    for(auto ele : adjacent_triangles) {
        if (*ele == *CT ) continue;
        Neigh=ele;
    }

    int nT_LedgeID     = Neigh->getLongestEdgeID();
    EdgeOrder nT_Ledge = Neigh->getEO(nT_LedgeID);
    NeighLedge         = nT_Ledge;

}
*/
HasCommonLE DesiredEdge2_1Send::hassameedges(EdgeOrder* ed1,EdgeOrder* ed2) {

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
//    pMesh->delCertainTriInalltriangles(T);

    upDateDS(T);
}

void DesiredEdge2_1Send::upDateDS(Triangle *T) {

    T->isAlive = false;
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


bool isExist(std::set<EdgeOrder*,CompareEdgeOrder> &container, EdgeOrder* key) {

    typedef std::set<EdgeOrder*,CompareEdgeOrder> ::iterator iter;
    iter it = container.find(key);
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


