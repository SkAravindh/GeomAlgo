#include "../Mesh.h"

enum class HasCommonLE {
    same,
    not_same
};


class DesiredEdge {
public:
    DesiredEdge() = default;
    DesiredEdge(std::shared_ptr<Mesh> &mesh);
    void do_refine();
    void split(EdgeOrder &C_edge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(EdgeOrder &currLongEdge, Triangle *currenT, Triangle *Neigh, EdgeOrder &NeighLongEdge,  bool flag=true);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle *currenT, Triangle *Neigh,  EdgeOrder &NeighLedge,bool flag=true);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh, EdgeOrder &NeighLedge);
    void deleteInfoFromDS(Triangle *T);
    void upDateDS(Triangle *T);
    void collapse(EdgeOrder &C_edge);
    void updatechildrened(std::vector< std::pair<EdgeOrder,EdgeOrder> > & oldtonew);

private:
    std::shared_ptr<Mesh> pMesh;
    std::set<EdgeOrder> children_ED;

    std::set<EdgeOrder> visited;
   // std::vector<EdgeOrder> input_edges;
    std::set<EdgeOrder> input_edges;
    std::set<EdgeOrder> nelongedge;
    std::set<EdgeOrder> tocollapse;

};

DesiredEdge::DesiredEdge(std::shared_ptr<Mesh> &mesh):pMesh(mesh) {}

void DesiredEdge::do_refine() {

    std::vector<Triangle*> all_triangles;
    std::vector<Triangle*> Ring;
    pMesh->getTriangles(all_triangles);
    pMesh->getNeigTrianglesbyOrder(all_triangles[0],3,Ring);
    //writeSTL("patch.stl", Ring);
   std::vector<EdgeOrder> input;
    getEdgesofTrianlges(Ring, input);
    for (auto in : input) {
        input_edges.insert(in);
    }
    for(int i=0; i<4; i++) {
        std::cout<< "iteration: "<<i<<" "<<input_edges.size()<<std::endl;
        int count = 0;
        for (auto edge : input_edges) {
            ++count;
         //   std::cout << "count " << count <<" "<<edge<< std::endl;

            auto it = visited.find(edge);
            if (it != visited.end()) {
                //     std::cout << "count " <<count<<"already splitted" << std::endl;
                continue;
            }
            double edlen = edge.getlength();
            if (edlen > (4.0/3.0)*0.3) {
                split(edge);
            //    std::cout <<"greater" <<edge.getlength() << std::endl;
            }

            // break;
           // std::cout << std::endl;
          //  std::cout << edge.getlength() << std::endl;
        }

        std::cout<<std::endl;

     //   std::cout << "neigh " <<nelongedge.size()<<" "<<children_ED.size()<< std::endl;
        for(auto nei : nelongedge) {
            children_ED.erase(nei);
        }
        for(auto in : input_edges) {
            children_ED.erase(in);
        }
        input_edges.clear();

     //   std::cout << "childsize after "<<children_ED.size()<< std::endl;
        visited.clear();
//
        tocollapse.clear();
        for(auto chiled : children_ED) {
            double len = chiled.getlength();
            if ( len < (4.0/5.0)*1.0 ) {
             tocollapse.insert(chiled);
               // collapse(chiled);
            }
        }


        while(!tocollapse.empty()) {
            EdgeOrder childdd = *tocollapse.begin();
          // std::cout << "collapse is happening " <<std::endl;
            collapse(childdd);
         //
       //  break;
        }
        for(auto ch : children_ED) {
            input_edges.insert(ch);
        }
    }
}

void DesiredEdge::collapse(EdgeOrder &C_edge) {

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
    }



    std::vector<EdgeOrder> edvec;
    getEdgesofTrianlges(adjusten_triangle, edvec);
    for (auto ced : edvec) {
        auto iit = children_ED.find(ced);
        if (iit != children_ED.end()) {
            children_ED.erase(ced);
            tocollapse.erase(ced);
        }
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


void DesiredEdge::updatechildrened(std::vector< std::pair<EdgeOrder,EdgeOrder> > & oldtonew) {
    for(auto ele : oldtonew) {
        auto iit = children_ED.find(ele.first);
        if(iit != children_ED.end()) {
            children_ED.erase(ele.first);
           children_ED.insert(ele.second);
           tocollapse.erase(ele.first);
        }
    }
}
void DesiredEdge::split(EdgeOrder &C_edge) {


    std::vector<Triangle*> c_edge_triangles;
    pMesh->getAdjustenNeigh(C_edge,c_edge_triangles);
    if(c_edge_triangles.size() != 2) {

        std::cout << "C_edge " <<C_edge<<std::endl;
        std::cout << "triangle vector size is not two at split function  " << std::endl;
       // return;
    }
    int adj_no=0;
    for(auto adj_tri : c_edge_triangles) {
        ++adj_no;
      //  std::cout << "adj no " << adj_no <<std::endl;
//        std::cout<<C_edge<<std::endl;
//        std::cout << *adj_tri << std::endl;
        Triangle* current_adjT(adj_tri);
        int nT_LedgeID     = current_adjT->getLongestEdgeID();
        EdgeOrder Neigh_Longestedge = current_adjT->getEO(nT_LedgeID);

        HasCommonLE re = hassameedges(C_edge,Neigh_Longestedge);

        if (re == HasCommonLE::same) {
       //     std::cout<<"split_2 begin " << std::endl;
            splitIntoTwo(C_edge, nullptr,current_adjT,Neigh_Longestedge);
        }
        else if (re == HasCommonLE::not_same) {
       //     std::cout<<"split_3 begin " << std::endl;
            splitIntoThree(C_edge, nullptr,current_adjT,Neigh_Longestedge);
        }

    }
}

void DesiredEdge::splitIntoTwo(EdgeOrder &currLongEdge, Triangle *currenT,Triangle *Neigh,EdgeOrder &NeighLongEdge, bool flag) {
   // std::cout<<"split_2 function " << std::endl;
    if (std::count(input_edges.begin(), input_edges.end(), NeighLongEdge)) {
        visited.insert(NeighLongEdge);
    }
    else {
        nelongedge.insert(NeighLongEdge);
        nelongedge.insert(currLongEdge);
    }
    Point* midpoint = currLongEdge.getMidPoint();

    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();
    Point* NeighT_peakvertex_ID =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* F_5 = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(midpoint,NeighT_peakvertex_ID,F_4,Neigh);
    pMesh->createTriangle(midpoint,F_5,NeighT_peakvertex_ID,Neigh);

    std::vector<Triangle*> childs;
    Neigh->getChildren(childs);
    std::vector<EdgeOrder> childedges;
    getEdgesofTrianlges(childs,childedges);


    for(auto c_ed : childedges) {

        auto iit = input_edges.find(c_ed);

        if(iit == input_edges.end()) {
        //    std::cout <<"from_2 "<< c_ed << std::endl;
            children_ED.insert(c_ed);
        }
//        if (!std::count(input_edges.begin(), input_edges.end(), c_ed)) {
//            children_ED.insert(c_ed);
//
//        }
    }
    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    deleteInfoFromDS(Neigh);
}
void DesiredEdge::splitIntoThree( EdgeOrder &currLongEdge, Triangle *currenT ,Triangle *Neigh,  EdgeOrder &NeighLedge,bool flag) {
   // std::cout<<"split_3 function " << std::endl;
    if (std::count(input_edges.begin(), input_edges.end(), NeighLedge)) {
        visited.insert(NeighLedge);
    } else {
        nelongedge.insert(NeighLedge);
        nelongedge.insert(currLongEdge);
    }

    Point* NeighT_LedgeMP     =  NeighLedge.getMidPoint();
    Point* CurrentT_LedgeMP   =  currLongEdge.getMidPoint();
    int NeighT_LedgeID        =  Neigh->getLongestEdgeID();
    Point* NeighT_peakVertex  =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = currLongEdge.p0;
    Point* F_5 = NeighLedge.p1;

    if (*currLongEdge.p0 == *NeighT_peakVertex) {
        F_4 = currLongEdge.p1;
    }

    if (*F_4 == *NeighLedge.p1) {
        F_5 = NeighLedge.p0;
    }

    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_peakVertex,NeighT_LedgeMP,Neigh);
    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_LedgeMP, F_4,Neigh);
    pMesh->createTriangle(NeighT_LedgeMP,NeighT_peakVertex,F_5,Neigh);

    std::vector<Triangle*> childs;
    Neigh->getChildren(childs);
    std::vector<EdgeOrder> childedges;
    getEdgesofTrianlges(childs,childedges);


    for(auto c_ed : childedges) {
        auto iit = input_edges.find(c_ed);
        if(iit == input_edges.end()) {
         //   std::cout <<"from_3 "<< c_ed << std::endl;
            children_ED.insert(c_ed);
        }
//        if (!std::count(input_edges.begin(), input_edges.end(), c_ed)) {
//            children_ED.insert(c_ed);
//
//        }
    }

    if(currenT != nullptr) {
        deleteInfoFromDS(currenT);
    }
    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge);

    HasCommonLE re = hassameedges(NeighLedge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge, true);
    }
    else if (re == HasCommonLE::not_same) {
        splitIntoThree(NeighLedge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
    }
}


HasCommonLE DesiredEdge::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void DesiredEdge::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh,
                                 EdgeOrder &NeighLedge) {

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

void DesiredEdge::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);
    upDateDS(T);


}

void DesiredEdge::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);

//    for (auto ele : TV) {
//        //   pMesh->establishNeighofTriangle(ele);
//    }
    pMesh->fillTriangleContainers(TV,alltri);

}