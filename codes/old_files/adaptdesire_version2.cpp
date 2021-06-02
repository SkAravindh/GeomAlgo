#include "../Mesh.h"

enum class HasCommonLE {
    same,
    not_same
};

class DesiredEdge_2 {
public:
    DesiredEdge_2() = default;
    DesiredEdge_2(std::shared_ptr<Mesh> &mesh);
    void do_refine();
    void split(EdgeOrder &edge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(EdgeOrder &currLongEdge,  Triangle* currenT,  Triangle *Neigh,  EdgeOrder &NeighLongest_edge);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle* currenT,  Triangle *Neigh,  EdgeOrder &NeighLongest_edge);
    bool isEdgeInside(EdgeOrder & edge, std::string messgae);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge);
    void collapse(EdgeOrder &C_edge);
    void updatechildrened(std::vector< std::pair<EdgeOrder,EdgeOrder> > & oldtonew);
    bool addedge(EdgeOrder &ed);
    bool addchild(EdgeOrder &ed);
    bool addchildout(EdgeOrder &ed);
    void deleteedgeinfo(EdgeOrder &ed);
    void deleteedgeinfooutside(EdgeOrder &ed);
    void removedupforcollapse(std::vector<EdgeOrder> &edge_vector);
    double aspectRatio(Triangle* t);

private:
    std::shared_ptr<Mesh> pMesh;
   // std::set<Point> input_edges_vertices;
   // std::set<EdgeOrder> input_edges;
    std::set<EdgeOrder> visited;

  //  std::set<EdgeOrder> childrenEdge_Inside;
//    std::set<EdgeOrder> childrenEdge_Outside;

    std::set<EdgeOrder> Neighlongedge;
  //

private: //trying
    std::map<EdgeOrder,EdgeOrder> edge_to_edge;
    std::vector<EdgeOrder> input_edges;
    std::vector<EdgeOrder> edgesofpatch;
    std::map<EdgeOrder,EdgeOrder> child_to_child;
    std::vector<EdgeOrder> children;

    std::map<EdgeOrder,EdgeOrder> child_to_child_out;
    std::vector<EdgeOrder> childrenoutside;
    std::vector<EdgeOrder> edges_to_collapse;

    std::map<EdgeOrder,EdgeOrder> ed_to_ed_collapse_map;


    std::vector<Triangle*> patchvector;
    std::vector<Triangle*> aspectTri;
private:
    //Iterators
    typedef std::set<Point>::iterator PIT;
    typedef std::map<EdgeOrder,EdgeOrder>::iterator EEMAP;
};
DesiredEdge_2::DesiredEdge_2(std::shared_ptr<Mesh> &mesh): pMesh(mesh) {}

void DesiredEdge_2::do_refine() {
    std::vector<Triangle *> all_triangle;
    pMesh->getTriangles(all_triangle);
    std::vector<Triangle *> Ring;
    pMesh->getNeigTrianglesbyOrder(all_triangle[0], 6, Ring);
     writeSTL("patch2.stl", Ring);

    getEdgesofTrianlges(Ring, edgesofpatch);
//    for(auto ed : edges) {
//        input_edges.insert(ed);
//    }

    std::vector<Point *> inputvertices;
    getBorderPoints(edgesofpatch, inputvertices);
//    for(auto vertex : inputvertices) {
//        input_edges_vertices.insert(*vertex);
//    }
    int count = 0;
    for (int i = 0; i < 3; i++) {
        std::cout << "Iteration: " << i << std::endl;
        for (auto current_edge : edgesofpatch) {  //while(!edgesofpatch.empty()) {
            //  EdgeOrder current_edge = *edgesofpatch.begin();
            if (addedge(current_edge)) continue;
            ++count;
            //  std::cout <<"count " <<count << std::endl;
            auto it = visited.find(current_edge);
            if (it != visited.end()) {
                //     std::cout << "count " <<count<<"already splitted" << std::endl;
                continue;
            }
            double current_edge_length = current_edge.getlength();
            if (current_edge_length > (4.0 / 3.0) * 0.3) {
                split(current_edge);
            }


        }

        //  aspectTri.clear();

        // writeSTL("Aspect.stl", aspectTri);

//        while (!aspectTri.empty()) {
//            Triangle* tri = *aspectTri.begin();
//            int edid = tri->getShortestEdgeID();
//            EdgeOrder ed = tri->getEO(edid);
//            collapse(ed);
//        }
        std::cout << "children " << children.size() << std::endl;
        edgesofpatch.clear();
        edgesofpatch.assign(children.begin(), children.end());
        children.clear();

//
//        for(auto ele : childrenoutside) {
//            if (!std::count(edgesofpatch.begin(), edgesofpatch.end(), ele)) {
//                edgesofpatch.push_back(ele);
//            }
//
//        }

//        to_collapse.clear();
//        std::vector<EdgeOrder> collapsevec1;
//        for(auto c_inside : edgesofpatch) {
//            double len = c_inside.getlength();
//            if ( len < (4.0/5.0)*1.0) {
//                to_collapse.insert(c_inside);
//                // collapse(chiled);
//            }
//        }
//        std::vector<EdgeOrder> collapsevec2;
//        for(auto c_outside : childrenoutside) {
//            double len = c_outside.getlength();
//            if ( len < (4.0/5.0)*1.0 ) {
//                to_collapse.insert(c_outside);
//                // collapse(chiled);
//            }
//        }
        //  removedupforcollapse(collapsevec1);
        //  removedupforcollapse(collapsevec2);
        //    std::cout << "collapsevec1 " <<collapsevec1.size() << " " <<"collapsevec2 "<<collapsevec2.size()<<std::endl;
//       int co=0;
//        while(!to_collapse.empty()) {
//            EdgeOrder childdd = *to_collapse.begin();
//            ++co;
//             std::cout << "co " << co<<std::endl;
//            collapse(childdd);
//            //
//             // break;
//        }





//        if (i==2) {
//            std::vector<EdgeOrder> edvec;
//            edvec.assign(childrenoutside.begin(), childrenoutside.end());
//            std::vector<Point *> pt;
//            getBorderPoints(edvec, pt);
//            writePoints("points.vtk", pt);
//        }
        std::cout << "size of the patch " << edgesofpatch.size() << " " << "childrenout" << childrenoutside.size()
                  << std::endl;
        std::cout << "size of collapse " << edges_to_collapse.size() << std::endl;
        childrenoutside.clear();
        // edges_to_collapse.clear();

    }
//    bool flag=true;
//    std::vector<Triangle*> delt;
//    int itercount =0;
//while(flag){
//    ++itercount;
//    std::cout << "itercount " <<itercount<< std::endl;
//    delt.clear();
//    for (auto as : patchvector) {
//        //std::cout<<"Aspect " <<aspectRatio(as)<< std::endl;
//        double aspect = aspectRatio(as);
//        if (0.3 > aspect || aspect > 1.5) {
//            delt.push_back(as);
//            int edid = as->getShortestEdgeID();
//            EdgeOrder ed = as->getEO(edid);
//            collapse(ed);
//        }
//    }
//    if(delt.empty()){
//        flag= false;
//    }
//}
  //  writeSTL("patch.stl", patchvector);
}


void DesiredEdge_2::collapse(EdgeOrder &C_edge) {

    std::map<std::string, Point *> string_to_point;
    string_to_point.insert(std::make_pair("p0", C_edge.p0));
    string_to_point.insert(std::make_pair("p1", C_edge.p1));
    Point *newpoint = C_edge.getMidPoint();

    // std::cout << "hi " << std::endl;
    std::vector<Triangle *> adjusten_triangle;
    pMesh->getAdjustenNeigh(C_edge, adjusten_triangle);
    std::set<Triangle> adt;
    for (auto t : adjusten_triangle) {
        adt.insert(*t);
        pMesh->delCertainEntryPT(t);
        pMesh->delCertainEntryET(t);
        pMesh->delCertainTriInalltriangles(t);
        eraseCertainTriangle(patchvector,t);
     //   eraseCertainTriangle(aspectTri,t);
    }



    std::vector<EdgeOrder> edvec;
    getEdgesofTrianlges(adjusten_triangle, edvec);
    for (auto ced : edvec) {
        if (std::count(children.begin(), children.end(), ced)) {
            children.erase(std::remove(children.begin(), children.end(), ced), children.end());
        }

//        auto iit = to_collapse.find(ced);
//        if (iit != to_collapse.end()) {
//            to_collapse.erase(ced);
//        }

    }


    for (auto inp : string_to_point) {
        std::vector<Triangle *> ringtri;
        pMesh->getRingNeigbyOrder(inp.second, 1, ringtri);
        for (auto ele : ringtri) {
            auto adit = adt.find(*ele);
            if (adit != adt.end()) {
                //  std::cout << "hiadg" << std::endl;
                continue;
            }
            std::vector<std::pair<EdgeOrder, EdgeOrder> > oldED_to_newED = pMesh->updateEdgeInfo(ele, inp.second,
                                                                                                 newpoint);
            updatechildrened(oldED_to_newED);
        }
    }


}


void DesiredEdge_2::updatechildrened(std::vector< std::pair<EdgeOrder,EdgeOrder> > & oldtonew) {
    for (auto ele : oldtonew) {
        if (std::count(children.begin(), children.end(), ele.first)) {
            children.erase(std::remove(children.begin(), children.end(), ele.first), children.end());
            children.push_back(ele.second);
        }

//        auto iit = to_collapse.find(ele.first);
//        if (iit != to_collapse.end()) {
//            to_collapse.erase(ele.first);
//        }
    }
}

void DesiredEdge_2::removedupforcollapse(std::vector<EdgeOrder> &edge_vector) {
    EEMAP itr;
    for(auto ele : edge_vector) {
        itr = ed_to_ed_collapse_map.find(ele);
        if (itr == ed_to_ed_collapse_map.end()) {
            edges_to_collapse.push_back(ele);
            ed_to_ed_collapse_map.insert(std::make_pair(ele,ele));
        }
    }
}
bool DesiredEdge_2::addedge(EdgeOrder &ed) {
    EEMAP iterMap;
    iterMap = edge_to_edge.find(ed);
    if (iterMap != edge_to_edge.end()) {
        return true;
    } else {
        input_edges.push_back(ed);
        edge_to_edge.insert(std::make_pair(ed,ed));
        return false;
    }
}

bool DesiredEdge_2::addchild(EdgeOrder &ed) {
    EEMAP iterMap;
    iterMap = child_to_child.find(ed);
    if (iterMap != child_to_child.end()) {
        return true;
    } else {
        children.push_back(ed);
        child_to_child.insert(std::make_pair(ed,ed));
        return false;
    }
}

bool DesiredEdge_2::addchildout(EdgeOrder &ed) {
    EEMAP iterMap;
    iterMap = child_to_child_out.find(ed);
    if (iterMap != child_to_child_out.end()) {
        return true;
    } else {
        childrenoutside.push_back(ed);
        child_to_child_out.insert(std::make_pair(ed,ed));
        return false;
    }
}

void DesiredEdge_2::deleteedgeinfo(EdgeOrder &ed) {
    child_to_child.erase(ed);
    children.erase(std::remove(children.begin(), children.end(), ed), children.end());
}

void DesiredEdge_2::deleteedgeinfooutside(EdgeOrder &ed) {
    child_to_child_out.erase(ed);
    childrenoutside.erase(std::remove(childrenoutside.begin(), childrenoutside.end(), ed), childrenoutside.end());
}

void DesiredEdge_2::split(EdgeOrder &edge) {
   // std::cout << "split function " << std::endl;
    std::vector<Triangle*> sharing_triangle;
    pMesh->getAdjustenNeigh(edge,sharing_triangle);
    if(sharing_triangle.size() != 2) {
        std::cout << "C_edge " <<edge<<std::endl;
        std::cout << "triangle vector size is not two at split function  " << std::endl;
        return;
    }

    for(auto triangle : sharing_triangle) {

        Triangle* current_neigh_triangle(triangle);
        int id = current_neigh_triangle->getLongestEdgeID();
        EdgeOrder current_neigh_triangle_longED = current_neigh_triangle->getEO(id);

        HasCommonLE re = hassameedges(edge,current_neigh_triangle_longED);
        if (re == HasCommonLE::same) {
            //     std::cout<<"split_2 begin " << std::endl;
            splitIntoTwo(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED);
        }
        else if (re == HasCommonLE::not_same) {
            //     std::cout<<"split_3 begin " << std::endl;
            splitIntoThree(edge, nullptr,current_neigh_triangle,current_neigh_triangle_longED);
        }
    }
}

void DesiredEdge_2::splitIntoTwo(EdgeOrder &currLongEdge, Triangle *currenT, Triangle *Neigh, EdgeOrder &NeighLongest_edge) {

//    if (std::count(edgesofpatch.begin(), edgesofpatch.end(), NeighLongest_edge)) {
//        visited.insert(NeighLongest_edge);
//    }
//    else {
//        //Neighlongedge.insert(currLongEdge);
//        Neighlongedge.insert(NeighLongest_edge);
//    }

//    if(currenT != nullptr) {
//        bool re_currLongEdge = isEdgeInside(currLongEdge, "SplitIntoTwo function");
//    }

//    bool re_NeighLongest_edge = isEdgeInside(NeighLongest_edge, "SplitIntoTwo function");
//    PIT iter_1;
//    if(re_NeighLongest_edge) {
//        Point *midpoint_neigh = NeighLongest_edge.getMidPoint();
//        iter_1 = input_edges_vertices.find(*midpoint_neigh);
//      //  if (*iter_1 == *input_edges_vertices.end()) {
//            input_edges_vertices.insert(*midpoint_neigh);
//      //  }
//    }
 //   std::cout << "split into three " << std::endl;
    Point* midpoint = currLongEdge.getMidPoint();

    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();
    Point* NeighT_peakvertex_ID =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* F_5 = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(midpoint,NeighT_peakvertex_ID,F_4,Neigh);
    pMesh->createTriangle(midpoint,F_5,NeighT_peakvertex_ID,Neigh);
    EdgeOrder ed_1(midpoint,F_5);
    EdgeOrder ed_2(midpoint,F_4);
    EdgeOrder ed_3(midpoint,NeighT_peakvertex_ID);

    if (std::count(edgesofpatch.begin(), edgesofpatch.end(), NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        addchild(ed_1);
        addchild(ed_2);
        addchild(ed_3);
    }
    else if(std::count(children.begin(), children.end(), NeighLongest_edge)) {
        deleteedgeinfo(NeighLongest_edge);
        addchild(ed_1);
        addchild(ed_2);
        addchild(ed_3);
    }
    else {
            deleteedgeinfooutside(NeighLongest_edge);
            addchildout(ed_1);
            addchildout(ed_2);
            addchildout(ed_3);

    }
//    std::vector<Triangle*> child;
//    Neigh->getChildren(child);
//    std::vector<EdgeOrder> childedges;
//    getEdgesofTrianlges(child,childedges);

//    for(auto c_ed : childedges) {
//        auto iit = input_edges.find(c_ed);
//        if(iit == input_edges.end()) {
//            //    std::cout <<"from_2 "<< c_ed << std::endl;
//            if(isEdgeInside(c_ed, "from child loop_splitIntoTwo")){
//                childrenEdge_Inside.insert(c_ed);
//            } else{
//                childrenEdge_Outside.insert(c_ed);
//            }
//        }
//    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    deleteInfoFromDS(Neigh);
}

void DesiredEdge_2::splitIntoThree(EdgeOrder &currLongEdge, Triangle *currenT, Triangle *Neigh, EdgeOrder &NeighLongest_edge) {

//    if (std::count(edgesofpatch.begin(), edgesofpatch.end(), NeighLongest_edge)) {
//        visited.insert(NeighLongest_edge);
//    }
//    if (std::count(edgesofpatch.begin(), edgesofpatch.end(), currLongEdge)) {
//        visited.insert(currLongEdge);
//    }
//
//    else {
//        Neighlongedge.insert(currLongEdge);
//        Neighlongedge.insert(NeighLongest_edge);
//    }
  //  bool re_currLongEdge = isEdgeInside(currLongEdge, "SplitIntoThree function edge_1");
   // bool re_NeighLongest_edge = isEdgeInside(NeighLongest_edge, "SplitIntoThree function edge_2");

//    PIT iter_1;
//    if(re_NeighLongest_edge) {
//        Point *midpoint_neigh = NeighLongest_edge.getMidPoint();
//        iter_1 = input_edges_vertices.find(*midpoint_neigh);
//        if (*iter_1 == *input_edges_vertices.end()) {
//            input_edges_vertices.insert(*midpoint_neigh);
//        }
//    }
//
//    PIT iter_2;
//    if(re_currLongEdge) {
//        Point *midpoint_curr = currLongEdge.getMidPoint();
//        iter_2 = input_edges_vertices.find(*midpoint_curr);
//        if (*iter_2 == *input_edges_vertices.end()) {
//            input_edges_vertices.insert(*midpoint_curr);
//        }
//    }

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

    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_peakVertex,NeighT_LedgeMP,Neigh);
    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_LedgeMP, F_4,Neigh);
    pMesh->createTriangle(NeighT_LedgeMP,NeighT_peakVertex,F_5,Neigh);

    EdgeOrder c_ed1(CurrentT_LedgeMP,NeighT_peakVertex);
    EdgeOrder c_ed2(CurrentT_LedgeMP,F_4);
    EdgeOrder c_ed3(CurrentT_LedgeMP,NeighT_LedgeMP);

    EdgeOrder n_ed1(NeighT_LedgeMP,F_5);
    EdgeOrder n_ed2(NeighT_LedgeMP,F_4);
    EdgeOrder n_ed3(NeighT_LedgeMP,NeighT_peakVertex);

    if (std::count(edgesofpatch.begin(), edgesofpatch.end(), NeighLongest_edge)) {
        visited.insert(NeighLongest_edge);
        addchild(n_ed1);
        addchild(n_ed2);
        addchild(n_ed3);
//
//        addchild(c_ed1);
//        addchild(c_ed2);
//        addchild(c_ed3);
    }
    else if (std::count(children.begin(), children.end(), NeighLongest_edge)) {
        deleteedgeinfo(NeighLongest_edge);
        addchild(n_ed1);
        addchild(n_ed2);
        addchild(n_ed3);

//        addchild(c_ed1);
//        addchild(c_ed2);
//        addchild(c_ed3);
    }else {

            deleteedgeinfooutside(NeighLongest_edge);
            addchildout(n_ed1);
            addchildout(n_ed2);
            addchildout(n_ed3);
    }
    if (std::count(edgesofpatch.begin(), edgesofpatch.end(), currLongEdge)) {
        visited.insert(currLongEdge);
        addchild(c_ed1);
        addchild(c_ed2);
        addchild(c_ed3);

//        addchild(n_ed1);
//        addchild(n_ed2);
//      addchild(n_ed3);

    }
     else if (std::count(children.begin(), children.end(), currLongEdge)) {
        deleteedgeinfo(currLongEdge);
        addchild(c_ed1);
        addchild(c_ed2);
        addchild(c_ed3);

//       addchild(n_ed1);
//       addchild(n_ed2);
//      addchild(n_ed3);
    }else {

            deleteedgeinfooutside(currLongEdge);
            addchildout(c_ed1);
            addchildout(c_ed2);
            addchildout(c_ed3);

    }

//std::cout << "split into three " << std::endl;
//    std::vector<Triangle*> childs;
//    Neigh->getChildren(childs);
//    std::vector<EdgeOrder> childedges;
//    getEdgesofTrianlges(childs,childedges);

//    for(auto c_ed : childedges) {
//        auto iit = input_edges.find(c_ed);
//        if(iit == input_edges.end()) {
//            if(isEdgeInside(c_ed, "from child loop_splitIntoTwo")){
//                childrenEdge_Inside.insert(c_ed);
//            } else{
//                childrenEdge_Outside.insert(c_ed);
//            }
//        }
//    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }

    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge);

    HasCommonLE re = hassameedges(NeighLongest_edge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLongest_edge,Neigh,second_direct_NeighT,second_NeighT_Ledge);
    }
    else if (re == HasCommonLE::not_same) {
        splitIntoThree(NeighLongest_edge, Neigh, second_direct_NeighT, second_NeighT_Ledge);
    }
}
bool DesiredEdge_2::isEdgeInside(EdgeOrder &edge, std::string messgae) {

    std::vector<Point*> NeighLongestPoints;
    NeighLongestPoints.push_back(edge.p0);
    NeighLongestPoints.push_back(edge.p1);
    PIT iter;
    int pointcount=0;
    for(auto Neigh_ED_Point : NeighLongestPoints) {
      //  iter  = input_edges_vertices.find(*Neigh_ED_Point);
       // if (*iter != *input_edges_vertices.end()) {
            ++pointcount;
        }
    //}
    if (pointcount == 2) {
//        Point *midpoint = edge.getMidPoint();
//        iter_1 = input_edges_vertices.find(*midpoint);
//        if (*iter_1 == *input_edges_vertices.end()) {
//            input_edges_vertices.insert(*midpoint);
//        }
        return true;
    }
    else if(pointcount == 0) {
    //    std::cout << "vertices count is zero from "<< messgae << std::endl;
        return false;
    }
    else if(pointcount == 1) {
     //   std::cout << "vertices count is one from "<< messgae << std::endl;
        return false;
    }
    else if(pointcount > 2) {
     //   std::cout << "vertices count is greater than 2 from "<< messgae << std::endl;
        return false;
    }
}

void DesiredEdge_2::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh(ed,adjacent_triangles);

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

HasCommonLE DesiredEdge_2::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void DesiredEdge_2::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);
    if(!patchvector.empty()) {
        eraseCertainTriangle(patchvector,T);
    }
    upDateDS(T);


}

void DesiredEdge_2::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);

//    for (auto ele : TV) {
//        //   pMesh->establishNeighofTriangle(ele);
//    }
for(auto ele : TV) {
    patchvector.push_back(ele);
}
    pMesh->fillTriangleContainers(TV,alltri);

}

double DesiredEdge_2::aspectRatio(Triangle *t) {
    double D1 = t->getEO(0).getlength();
    double D2 = t->getEO(1).getlength();
    double D3 = t->getEO(2).getlength();
    double s= (D1+D2+D3)/2;
    double AR = (D1*D2*D3)/(8*(s-D1)*(s-D2)*(s-D3));
    return AR;
}
//std::cout << "iniside " <<childrenEdge_Inside.size()<<std::endl;
//std::cout << "outside " << childrenEdge_Outside.size() << std::endl;
//std::vector<EdgeOrder> insideedge;
//std::vector<EdgeOrder> outsideedge;
//insideedge.assign(childrenEdge_Inside.begin(), childrenEdge_Inside.end());
//outsideedge.assign(childrenEdge_Outside.begin(),childrenEdge_Outside.end());
//std::vector<Point*> insidepoint;
//std::vector<Point*> outsidepoint;
//getBorderPoints(insideedge,insidepoint);
//getBorderPoints(outsideedge,outsidepoint);
//writePoints("inside.vtk",insidepoint);
//writePoints("outside.vtk", outsidepoint);