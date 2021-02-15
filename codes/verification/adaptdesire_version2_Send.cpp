#include "../Mesh.h"


enum class HasCommonLE {
    same,
    not_same
};

template<class T_key, class T_value>
bool avoidDuplicates(std::map<T_key,T_value> &EE_Map, std::vector<T_key> &Evec, T_key &edge);

template<class T_key, class T_value>
bool deleteChildInfo(std::map<T_key,T_value> &EE_Map, std::vector<T_key> &Evec, T_key &edge);

bool isborder(std::set<EdgeOrder> &border_edges, const EdgeOrder &ed);

class DesiredEdge2Send {
public:
    DesiredEdge2Send()=default;
    DesiredEdge2Send(std::shared_ptr<Mesh> &mesh);
    void doRefine();
    void split(EdgeOrder &edge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(EdgeOrder &currLongEdge,  Triangle* currenT,  Triangle *Neigh,  EdgeOrder &NeighLongest_edge, bool flag);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle* currenT,  Triangle *Neigh,  EdgeOrder &NeighLongest_edge,bool flag);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge);
    void changedrs(Triangle* t, EdgeOrder &ed, EdgeOrder &neigh);


private:
    std::shared_ptr<Mesh> pMesh;

    std::vector<EdgeOrder> edges_to_Split;  //edgesofpatch.
    std::map<EdgeOrder,EdgeOrder> splitEdges_to_splitEdges_Map; //edge_to_edge.
    std::vector<EdgeOrder> alledges;
    std::set<EdgeOrder> visited; //to avoid repeating edges.

    //container to store edges inside given patch
    std::map<EdgeOrder,EdgeOrder> childIN_to_childIN;
    std::vector<EdgeOrder> children_Inside;

    //container to store edges outside given patch
    std::map<EdgeOrder,EdgeOrder> childOut_to_chilOut;
    std::vector<EdgeOrder> children_outside;
    std::set<EdgeOrder> border_edges;

    //
    std::set<Triangle*> outTriangle; //children of input patch.
    std::set<Triangle*> trackTriangle;
    //Iterators
    typedef std::vector<EdgeOrder>::iterator VEO;
    typedef std::map<EdgeOrder,EdgeOrder>::iterator MEE;
    typedef std::set<EdgeOrder>::iterator SEO;
};
DesiredEdge2Send::DesiredEdge2Send(std::shared_ptr<Mesh> &mesh) : pMesh(mesh) {}

void DesiredEdge2Send::doRefine() {

    std::vector<Triangle *> all_triangle;
    pMesh->getTriangles(all_triangle);
    std::vector<EdgeOrder> BE;
    getedgesByOrder(all_triangle,-1,BE);
    for(auto B_edges : BE) {
        border_edges.insert(B_edges);
    }

    std::vector<Triangle *> Ring;
    pMesh->getNeigTrianglesbyOrder(all_triangle[0], 5, Ring);
  //  writeSTL("patch.stl", Ring);
    for (auto ele : Ring) {
        trackTriangle.insert(ele);
    }
    getEdgesofTrianlges(Ring, edges_to_Split);

    VEO iter;
    for (int i = 0; i < 6; i++) {
        std::cout << "Iteration: " << i << std::endl;
        for (iter = edges_to_Split.begin(); iter != edges_to_Split.end(); iter++) {
            bool re = avoidDuplicates(splitEdges_to_splitEdges_Map,alledges,*iter);
            if(re) continue;
            SEO Sit = visited.find(*iter);
            if (Sit != visited.end()) continue;

            double current_edge_length = iter->getlength();
            if (current_edge_length > (4.0 / 3.0) * 1) {
                    split(*iter);
            }
        }

        std::cout << "children " << children_Inside.size() << std::endl;
        edges_to_Split.clear();
        edges_to_Split.assign(children_Inside.begin(), children_Inside.end());
        children_Inside.clear();
        std::cout << "size of the patch " << edges_to_Split.size() << " " << "childrenout" << children_outside.size()<< std::endl;
        children_outside.clear();
        if (edges_to_Split.empty()) {
            std::cout<< "Stop at iteration: " << i << " " << " Since it is reached given edge threshold " << std::endl;
            break;
        }

    }
    std::vector<Triangle*> outvec;
    outvec.assign(outTriangle.begin(),outTriangle.end());
   // writeSTL("outtri.stl", outvec);
}

void DesiredEdge2Send::split(EdgeOrder &edge) {

    std::vector<Triangle*> sharing_triangle;
    pMesh->getAdjustenNeigh_1(edge,sharing_triangle);

    if( !( (sharing_triangle.size() == 1 && isborder(border_edges,edge)) || sharing_triangle.size() == 2) ) {
        std::cout << "C_edge " <<edge<<std::endl;
        std::cout << "triangle vector size is not either two or one at split function  " << std::endl;
        return;
    }

    for(auto triangle : sharing_triangle) {

        Triangle* current_neigh_triangle(triangle);
        int id = current_neigh_triangle->getLongestEdgeID();
        EdgeOrder current_neigh_triangle_longED = current_neigh_triangle->getEO(id);

        HasCommonLE re = hassameedges(edge,current_neigh_triangle_longED);
        if (re == HasCommonLE::same) {
            //     std::cout<<"split_2 begin " << std::endl;
            splitIntoTwo(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED,true);
        } else if (isborder(border_edges ,current_neigh_triangle_longED) && !isborder(border_edges ,edge) ) {
             changedrs(current_neigh_triangle, edge,current_neigh_triangle_longED );
        }
        else if (isborder(border_edges ,current_neigh_triangle_longED) && isborder(border_edges ,edge)) {
            splitIntoTwo(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED, false);
        }
        else if (re == HasCommonLE::not_same) {
            //     std::cout<<"split_3 begin " << std::endl;
            splitIntoThree(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED,true);
        }
    }
}

void DesiredEdge2Send::changedrs(Triangle* t, EdgeOrder &ed, EdgeOrder &neigh) {
    splitIntoTwo(ed, nullptr,t,neigh,false);
}

void DesiredEdge2Send::splitIntoTwo(EdgeOrder &currLongEdge, Triangle *currenT, Triangle *Neigh, EdgeOrder &NeighLongest_edge,bool flag) {

    Point* midpoint = currLongEdge.getMidPoint();

    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();
    if(!flag) {
        NeighT_LedgeID = Neigh->getPeakVertexID(currLongEdge);
    }
    Point* NeighT_peakvertex_ID =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* F_5 = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(midpoint,NeighT_peakvertex_ID,F_4,Neigh);
    pMesh->createTriangle(NeighT_peakvertex_ID,midpoint,F_5,Neigh);
    //pMesh->createTriangle(midpoint,NeighT_peakvertex_ID,F_4,Neigh);
    //pMesh->createTriangle(F_5,NeighT_peakvertex_ID,midpoint,Neigh);
    EdgeOrder ed_1(midpoint,F_5);
    EdgeOrder ed_2(midpoint,F_4);
    EdgeOrder ed_3(midpoint,NeighT_peakvertex_ID);

    if(isborder(border_edges ,currLongEdge)) {
        border_edges.insert(EdgeOrder(midpoint,currLongEdge.p0));
        border_edges.insert(EdgeOrder(midpoint,currLongEdge.p1));
    }

    if (std::count(edges_to_Split.begin(), edges_to_Split.end(), NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        avoidDuplicates(childIN_to_childIN,children_Inside,ed_1);
        avoidDuplicates(childIN_to_childIN,children_Inside,ed_2);
        avoidDuplicates(childIN_to_childIN,children_Inside,ed_3);
    }else if (std::count(children_Inside.begin(), children_Inside.end(), NeighLongest_edge)) {
        avoidDuplicates(childIN_to_childIN,children_Inside,ed_1);
        avoidDuplicates(childIN_to_childIN,children_Inside,ed_2);
        avoidDuplicates(childIN_to_childIN,children_Inside,ed_3);
    }
    else {
        deleteChildInfo(childOut_to_chilOut,children_outside,NeighLongest_edge);
        avoidDuplicates(childOut_to_chilOut,children_outside,ed_1);
        avoidDuplicates(childOut_to_chilOut,children_outside,ed_2);
        avoidDuplicates(childOut_to_chilOut,children_outside,ed_3);
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    deleteInfoFromDS(Neigh);

}

void DesiredEdge2Send::splitIntoThree(EdgeOrder &currLongEdge, Triangle *currenT, Triangle *Neigh, EdgeOrder &NeighLongest_edge,bool flag) {

    Point* NeighT_LedgeMP     =  NeighLongest_edge.getMidPoint();
    Point* CurrentT_LedgeMP   =  currLongEdge.getMidPoint();
    int NeighT_LedgeID        =  Neigh->getLongestEdgeID();
    Point* NeighT_peakVertex  =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = currLongEdge.p0;
    Point* F_5 = NeighLongest_edge.p1;

    if (*currLongEdge.p0 == *NeighT_peakVertex) {
        F_4 = currLongEdge.p1;
    }

    if (*F_4 == *NeighLongest_edge.p1) {
        F_5 = NeighLongest_edge.p0;
    }

    pMesh->createTriangle(NeighT_peakVertex,CurrentT_LedgeMP,NeighT_LedgeMP,Neigh);
    pMesh->createTriangle(NeighT_peakVertex,NeighT_LedgeMP,F_5,Neigh);
    pMesh->createTriangle(F_4,NeighT_LedgeMP,CurrentT_LedgeMP, Neigh);

//    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_peakVertex,NeighT_LedgeMP,Neigh);
//    pMesh->createTriangle(NeighT_LedgeMP,NeighT_peakVertex,F_5,Neigh);
//    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_LedgeMP, F_4,Neigh);

    if ( isborder(border_edges,currLongEdge)) {
        border_edges.insert(EdgeOrder(CurrentT_LedgeMP,currLongEdge.p0));
        border_edges.insert(EdgeOrder(CurrentT_LedgeMP,currLongEdge.p1));
    }

    EdgeOrder c_ed1(CurrentT_LedgeMP,NeighT_peakVertex);
    EdgeOrder c_ed2(CurrentT_LedgeMP,F_4);
    EdgeOrder c_ed3(CurrentT_LedgeMP,NeighT_LedgeMP);

    EdgeOrder n_ed1(NeighT_LedgeMP,F_5);
    EdgeOrder n_ed2(NeighT_LedgeMP,F_4);
    EdgeOrder n_ed3(NeighT_LedgeMP,NeighT_peakVertex);

    if (std::count(edges_to_Split.begin(), edges_to_Split.end(), NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        avoidDuplicates(childIN_to_childIN,children_Inside,n_ed1);
        avoidDuplicates(childIN_to_childIN,children_Inside,n_ed2);
        avoidDuplicates(childIN_to_childIN,children_Inside,n_ed3);
    } else if (std::count(children_Inside.begin(), children_Inside.end(), NeighLongest_edge)) {
        deleteChildInfo(childIN_to_childIN,children_Inside,NeighLongest_edge);
        avoidDuplicates(childIN_to_childIN,children_Inside,n_ed1);
        avoidDuplicates(childIN_to_childIN,children_Inside,n_ed2);
        avoidDuplicates(childIN_to_childIN,children_Inside,n_ed3);
    }
    else {
        deleteChildInfo(childOut_to_chilOut,children_outside,NeighLongest_edge);
        avoidDuplicates(childOut_to_chilOut,children_outside,n_ed1);
        avoidDuplicates(childOut_to_chilOut,children_outside,n_ed2);
        avoidDuplicates(childOut_to_chilOut,children_outside,n_ed3);
    }

    if (std::count(edges_to_Split.begin(), edges_to_Split.end(), currLongEdge)) {
        visited.insert(currLongEdge);
        avoidDuplicates(childIN_to_childIN,children_Inside,c_ed1);
        avoidDuplicates(childIN_to_childIN,children_Inside,c_ed2);
        avoidDuplicates(childIN_to_childIN,children_Inside,c_ed3);
    } else if (std::count(children_Inside.begin(), children_Inside.end(), currLongEdge)) {
        deleteChildInfo(childIN_to_childIN,children_Inside,currLongEdge);
        avoidDuplicates(childIN_to_childIN,children_Inside,c_ed1);
        avoidDuplicates(childIN_to_childIN,children_Inside,c_ed2);
        avoidDuplicates(childIN_to_childIN,children_Inside,c_ed3);
    }
    else {
        deleteChildInfo(childOut_to_chilOut,children_outside,currLongEdge);
        avoidDuplicates(childOut_to_chilOut,children_outside,c_ed1);
        avoidDuplicates(childOut_to_chilOut,children_outside,c_ed2);
        avoidDuplicates(childOut_to_chilOut,children_outside,c_ed3);
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }

    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge);

    HasCommonLE re = hassameedges(NeighLongest_edge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge ,true);
    }
    else if(isborder(border_edges,second_NeighT_Ledge )) {
        splitIntoTwo(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge, false);
    }
    else if (re == HasCommonLE::not_same) {
        splitIntoThree(NeighLongest_edge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
    }
}

void DesiredEdge2Send::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh_1(ed,adjacent_triangles);

    if( !( (adjacent_triangles.size() == 1 && isborder(border_edges,ed)) || adjacent_triangles.size() == 2) ) {
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

HasCommonLE DesiredEdge2Send::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void DesiredEdge2Send::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);

//    if (!outTriangle.empty()) {
//        outTriangle.erase(T);
//    }

    upDateDS(T);
}

void DesiredEdge2Send::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);

//    auto it = trackTriangle.find(T);
//    if (it != trackTriangle.end()) {
//       for (auto ele : TV) {
//           trackTriangle.insert(ele);
//           outTriangle.insert(ele);
//       }
//    }

    pMesh->fillTriangleContainers(TV,alltri);

}




template<class T_key, class T_value>
bool avoidDuplicates(std::map<T_key,T_value> &EE_Map, std::vector<T_key> &Evec, T_key &edge) {

    typedef typename std::map<T_key,T_value>::iterator it;
    it iter;
    iter = EE_Map.find(edge);
    if (iter != EE_Map.end()) {
        return true;
    }
    else {
        Evec.push_back(edge);
        EE_Map.insert(std::make_pair(edge,edge));
        return false;
    }
}

template<class T_key, class T_value>
bool deleteChildInfo(std::map<T_key,T_value> &EE_Map, std::vector<T_key> &Evec, T_key &edge) {
    EE_Map.erase(edge);
    Evec.erase(std::remove(Evec.begin(), Evec.end(), edge), Evec.end());
    return true;
}

bool isborder(std::set<EdgeOrder> &border_edges, const EdgeOrder &ed) {

    std::set<EdgeOrder>::iterator it;
    it = border_edges.find(ed);
    if (it != border_edges.end()) {
        return true;
    }
    else{
        return false;
    }

}

