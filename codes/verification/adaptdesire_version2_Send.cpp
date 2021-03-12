#include "../Mesh.h"
#include <queue>

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
    void doRefine(int a);
    void split(EdgeOrder &edge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(EdgeOrder &currLongEdge,  Triangle* currenT,  Triangle *Neigh,  EdgeOrder &NeighLongest_edge, bool flag);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle* currenT,  Triangle *Neigh,  EdgeOrder &NeighLongest_edge,bool flag);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge);
    void changedrs(Triangle* t, EdgeOrder &ed, EdgeOrder &neigh);
    void findTriToFixWindError();
    void fixWindError( Triangle* inside, const std::vector<EdgeOrder> &outtriangles_BoarderEdges);
    void flipEdge(Triangle* to_change, Triangle* parent, const EdgeOrder &commonedge);
    void fixWindError_1( Triangle* inside, const std::vector<EdgeOrder> &outtriangles_BoarderEdges);
    void checkNormal(Triangle* t);
    void flipNormal(Triangle* t);

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
    std::vector<Triangle*> outTrianglevector;
    bool hasborderEdge = false;
    bool enablechildrenout = false;
    //Iterators
    typedef std::vector<EdgeOrder>::iterator VEO;
    typedef std::map<EdgeOrder,EdgeOrder>::iterator MEE;
    typedef std::set<EdgeOrder>::iterator SEO;
};
DesiredEdge2Send::DesiredEdge2Send(std::shared_ptr<Mesh> &mesh) : pMesh(mesh) {}

void DesiredEdge2Send::doRefine(int a) {

    std::vector<Triangle *> all_triangle;
    pMesh->getTriangles(all_triangle);
    std::vector<EdgeOrder> BE;
    getedgesByOrder(all_triangle,-1,BE);

    if (!BE.empty()) {
        for(auto B_edges : BE) {
            border_edges.insert(B_edges);
        }
        hasborderEdge = true;
    }

   //std::vector<Triangle *> Ring;
   // pMesh->getNeigTrianglesbyOrder(all_triangle[a], 10, Ring); //
  //  writeSTL("patch.stl", Ring);
//    for (auto ele : Ring) {
//      //  trackTriangle.insert(ele);
//    }
    getEdgesofTrianlges(all_triangle, edges_to_Split);

    VEO iter;

    int c=0;
    for (int i = 0; i < 2; i++) {
        std::cout << "Iteration: " << i << std::endl;
        for (iter = edges_to_Split.begin(); iter != edges_to_Split.end(); iter++) {
            bool re = avoidDuplicates(splitEdges_to_splitEdges_Map,alledges,*iter);
            if(re) continue;
            SEO Sit = visited.find(*iter);
            if (Sit != visited.end()) continue;

            double current_edge_length = iter->getlength();

            if (current_edge_length > (4.0 / 3.6) *0.3 ) {

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
std::cout << "c " <<c << std::endl;
    //   findTriToFixWindError();
      //  outTriangle.clear();
   // std::vector<Triangle*> outvec;
   // outvec.assign(outTriangle.begin(),outTriangle.end());
  // writeSTL("outtri.stl", outvec);
    //std::cout << "outvec " << outTrianglevector.size() << std::endl;
}

void DesiredEdge2Send::split(EdgeOrder &edge) {

    std::vector<Triangle*> sharing_triangle;
    pMesh->getAdjustenNeigh_1(edge,sharing_triangle);

    if( !( (sharing_triangle.size() == 1 && isborder(border_edges,edge)) || sharing_triangle.size() == 2) ) {
     //   std::cout << "C_edge " <<edge<<std::endl;
     //   std::cout << "triangle vector size is not either two or one at split function  " << std::endl;
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
        } /*else if (isborder(border_edges ,current_neigh_triangle_longED) && !isborder(border_edges ,edge) ) {
             changedrs(current_neigh_triangle, edge,current_neigh_triangle_longED );
        }
        else if (hasborderEdge) {
            if (isborder(border_edges, current_neigh_triangle_longED) && isborder(border_edges, edge)) {
                splitIntoTwo(edge, nullptr, current_neigh_triangle, current_neigh_triangle_longED, false);
            }
        }*/
//        else if ( (isborder(border_edges,edge)) && (isborder(border_edges,current_+neigh_triangle_longED)) ) {
//            splitIntoTwo(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED, false);
//        }
//        else if ( (!isborder(border_edges,edge)) && (isborder(border_edges,current_neigh_triangle_longED)) ) {
//            splitIntoTwo(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED,true);
//        }
        else if (re == HasCommonLE::not_same) {
            //     std::cout<<"split_3 begin " << std::endl;
            splitIntoThree(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED,true);
        }
    }
}

void DesiredEdge2Send::changedrs(Triangle* t, EdgeOrder &ed, EdgeOrder &neigh) {
    splitIntoTwo(ed, nullptr,t,neigh,false);
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
    checkNormal(Neigh);
    EdgeOrder ed_1(midpoint,F_5);
    EdgeOrder ed_2(midpoint,F_4);
    EdgeOrder ed_3(midpoint,NeighT_peakvertex_ID);

    if(!flag) {
        if (isborder(border_edges, currLongEdge)) {
            border_edges.insert(EdgeOrder(midpoint, currLongEdge.p0));
            border_edges.insert(EdgeOrder(midpoint, currLongEdge.p1));
        }
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
    else if (enablechildrenout){
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
    checkNormal(Neigh);
//    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_peakVertex,NeighT_LedgeMP,Neigh);
//    pMesh->createTriangle(NeighT_LedgeMP,NeighT_peakVertex,F_5,Neigh);
//    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_LedgeMP, F_4,Neigh);

//    if ( isborder(border_edges,currLongEdge)) {
//        border_edges.insert(EdgeOrder(CurrentT_LedgeMP,currLongEdge.p0));
//        border_edges.insert(EdgeOrder(CurrentT_LedgeMP,currLongEdge.p1));
//    }

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
    else if(enablechildrenout){
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
 /*   else if (hasborderEdge) {
        if (isborder(border_edges, second_NeighT_Ledge)) {
            splitIntoTwo(NeighLongest_edge, Neigh, second_direct_NeighT, second_NeighT_Ledge, false);
        }
    }*/
//    else if( isborder(border_edges, second_NeighT_Ledge) ) {
//        splitIntoTwo(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge ,false);
//    }
    else if (re == HasCommonLE::not_same) {
        splitIntoThree(NeighLongest_edge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
    }
}

void DesiredEdge2Send::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh_1(ed,adjacent_triangles);

    if( !( (adjacent_triangles.size() == 1 && isborder(border_edges,ed)) || adjacent_triangles.size() == 2) ) {
        std::cout<<"adjacent_triangles.size() " <<adjacent_triangles.size()<< std::endl;

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
void DesiredEdge2Send::checkNormal(Triangle *t) {
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

void  DesiredEdge2Send::flipNormal(Triangle *t) {
    Point* vertex0 = t->getCorners(0);
    Point* vertex1 = t->getCorners(1);
    t->setNewVertex(vertex1,0);
    t->setNewVertex(vertex0,1);
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

   if (!outTrianglevector.empty()) {
        eraseCertainTriangle(outTrianglevector,T);
    }

    upDateDS(T);
}

void DesiredEdge2Send::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);

//    auto it = trackTriangle.find(T);
//    if (it != trackTriangle.end()) {
      for (auto ele : TV) {
          // trackTriangle.insert(ele);
          outTrianglevector.push_back(ele);
       }
//    }

    pMesh->fillTriangleContainers(TV,alltri);

}



void DesiredEdge2Send::findTriToFixWindError() {

    std::vector<EdgeOrder> outtriangles_BoarderEdges;
  // std::vector<Triangle*> outtriangles_vector;
   // outtriangles_vector.assign(outTriangle.begin(),outTriangle.end());
    getedgesByOrder(outTrianglevector,-1,outtriangles_BoarderEdges);
    std::cout << "outtriangles_BoarderEdges " <<outtriangles_BoarderEdges.size() << std::endl;
    //fixWindError(outTrianglevector.at(0),outtriangles_BoarderEdges);

    bool flag = true;
    int ID=0;
    Triangle* outside = nullptr;
    Triangle* inside = nullptr;
    EdgeOrder ed;
    while (flag) {
        if (ID >= outtriangles_BoarderEdges.size() ) {
            std::cout << "ID " <<ID << std::endl;
            break;
        }
        ed = outtriangles_BoarderEdges.at(ID);
        ++ID;
        std::vector<Triangle *> sharing_triangles;
        pMesh->getAdjustenNeigh_1(ed, sharing_triangles);
        if (sharing_triangles.size() != 2) continue;
        auto it = outTriangle.find(sharing_triangles[0]);
        if (it != outTriangle.end()) {
            inside = sharing_triangles[0];
            outside = sharing_triangles[1];
            flag = false;
        } else {

            inside = sharing_triangles[1];
            outside = sharing_triangles[0];
            flag = false;
       }
   }
    if (inside == nullptr && outside == nullptr) {
        fixWindError(outTrianglevector.at(0),outtriangles_BoarderEdges);
    }

   if (checkForHalfEdge(inside,outside)) {
       std::cout << "has half edge " << std::endl;
       } else {
       std::cout << "no half edge " << std::endl;
       flipEdge(inside,outside,ed);
       }
    fixWindError(inside,outtriangles_BoarderEdges);

}

void DesiredEdge2Send::flipEdge(Triangle* to_change, Triangle* parent, const EdgeOrder &commonedge) {

    int p0_ID = to_change->getVertexID(commonedge.p0);
    int p1_ID = to_change->getVertexID(commonedge.p1);
    to_change->setNewVertex(commonedge.p1,p0_ID);
    to_change->setNewVertex(commonedge.p0,p1_ID);

}

void DesiredEdge2Send::fixWindError( Triangle* inside, const std::vector<EdgeOrder> &outtriangles_BoarderEdges) {

  //  std::set<Triangle*> tempOutTriangles(outTriangle.begin(),outTriangle.end());
    std::vector<Triangle*> tempOutTriangles(outTrianglevector.begin(),outTrianglevector.end());
    std::set<EdgeOrder> tempBoarderEdges(outtriangles_BoarderEdges.begin(),outtriangles_BoarderEdges.end());
    std::cout << "tempBoarderEdges "<<tempBoarderEdges.size() << "tempOutTriangles "<<tempOutTriangles.size() << std::endl;
    std::queue<Triangle*> visited;
    visited.push(inside);
    int count=0;
    while (true) {
        if(tempOutTriangles.empty()) {
            break;
        }
        ++count;
        Triangle* parent = visited.front();
        visited.pop();
        for(int i=0; i<3; i++) {
            std::vector<Triangle*> sharing_triangle;
            EdgeOrder ed = parent->getEO(i);
            auto it = tempBoarderEdges.find(ed);
            if (it != tempBoarderEdges.end()) continue;
            pMesh->getAdjustenNeigh_1(ed,sharing_triangle);
            for (auto tri_to_check : sharing_triangle) {
                if (*parent == *tri_to_check) continue;
                //auto iter = tempOutTriangles.find(tri_to_check);
                //if ( iter == tempOutTriangles.end()) continue;
                if( !(std::count(tempOutTriangles.begin(), tempOutTriangles.end(), tri_to_check)) ) continue;
                if (!checkForHalfEdge(parent,tri_to_check)) {
                    flipEdge(tri_to_check,parent,ed);
                }
                visited.push(tri_to_check);
                eraseCertainTriangle(tempOutTriangles,tri_to_check);
               // tempOutTriangles.erase(tri_to_check);
            }
        }
       // tempOutTriangles.erase(parent);
        eraseCertainTriangle(tempOutTriangles,parent);
    }
}

void DesiredEdge2Send::fixWindError_1( Triangle* inside, const std::vector<EdgeOrder> &outtriangles_BoarderEdges) {

    std::vector<Triangle*> tempOuttri;
    tempOuttri.assign(outTrianglevector.begin(),outTrianglevector.end());
    std::set<EdgeOrder> tempBoarderEdges(outtriangles_BoarderEdges.begin(),outtriangles_BoarderEdges.end());
    std::queue<Triangle*> visited;
    visited.push(inside);
    while (true) {
        if(tempOuttri.empty()) {
            break;
        }
        Triangle* parent = visited.front();
        visited.pop();
        std::map<EdgeOrder,Triangle*> neighTmap;
        for (int i=0; i<3; i++) {
            EdgeOrder ed= parent->getEO(i);
            auto it = tempBoarderEdges.find(ed);
            if (it != tempBoarderEdges.end()) continue;
            std::vector<Triangle*> sharing_tri;
            pMesh->getAdjustenNeigh_1(ed,sharing_tri);
            for (auto tri_to_check : sharing_tri) {
                if (*parent == *tri_to_check) continue;
                if( !(std::count(tempOuttri.begin(), tempOuttri.end(), tri_to_check)) ) continue;
                neighTmap.insert(std::make_pair(ed,tri_to_check));
            }
        }
        Vector3 partentNormal = parent->getNormalVector();
        for(auto trimap : neighTmap) {
            eraseCertainTriangle(tempOuttri,trimap.second);
            visited.push(trimap.second);
            Vector3 neighNormal = trimap.second->getNormalVector();
            double PdotNeigh = dot(partentNormal,neighNormal);
            if (PdotNeigh < 0 ) {
                flipEdge(trimap.second,parent,trimap.first);
            }
        }
        eraseCertainTriangle(tempOuttri,parent);
    }

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


