
#include "Mesh.h"
#include "Edge.h"


//double truncDecimal(double d)
//{
//    return (d>0) ? floor(d) : ceil(d) ;
//}
//
//
//struct CompareFloatingPoint {
//
//   bool operator () (const Point* lhs, const Point* rhs) const {
//      return  std::make_tuple(truncDecimal(100. * lhs->x()), truncDecimal(100. * lhs->y()), truncDecimal(100. * lhs->z())) <
//       std::make_tuple(truncDecimal(100. * rhs->x()), truncDecimal(100. * rhs->y()), truncDecimal(100. * rhs->z()));
//   }
//
//
//};
enum class HasCommonLE {
    same,
    not_same
};

class AdaptiveTriangle {
public:
    AdaptiveTriangle(std::shared_ptr<Mesh> &mesh);
    void edgeRefinement();
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle* CT,   Triangle* & Neigh, EdgeOrder &NeighLedge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void dividethree( EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge);
    void dividetwo(EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge);
    void delInfoInDS(Triangle* T);
    void upDateDS(Triangle* T);
private:
    std::shared_ptr<Mesh> pMesh;
    std::set<Triangle*> parents;
    std::set<Triangle*> childinfo;
    typedef std::set<Triangle*>::iterator sit;
};

AdaptiveTriangle::AdaptiveTriangle(std::shared_ptr<Mesh> &mesh) {
    pMesh = mesh;
}

void AdaptiveTriangle::edgeRefinement() {
    std::vector<Triangle*> trianglevector;
    pMesh->getTriangles(trianglevector);
   // std::cout << "trivec " << *trianglevector[6000] << std::endl;
    int count =0;
    std::vector<Triangle*> Input_triangles;
    Input_triangles.push_back(trianglevector[5300]);
    while(!Input_triangles.empty()) {
        Triangle* CT = *Input_triangles.begin();
        eraseCertainTriangle(Input_triangles,CT);
      // std::cout << "CT " <<*CT << std::endl;
       std::set<Triangle*>::iterator it;
       it = parents.find(CT);
       if (it != parents.end()) {
           continue;
       }
        //std::cout << *current_Triangle << std::endl;

        int cT_LedgeID     = CT->getLongestEdgeID();
        EdgeOrder cT_Ledge = CT->getEO(cT_LedgeID);
        int cT_peakVertexID = CT->getPeakVertexID(cT_Ledge);
        Point* cT_edgeMP   = cT_Ledge.getMidPoint();
        Point* peakVertex     = CT->getCorners(cT_peakVertexID);
        Point* p1 = CT->getCorners(indexOrder_1(cT_peakVertexID));
        Point* p2 = CT->getCorners(indexOrder_2(cT_peakVertexID));
        //Splitting the current trianglel;
        Triangle* cT_child1 = pMesh->createTriangle(cT_edgeMP,p2,peakVertex,CT);
        Triangle* cT_child2 = pMesh->createTriangle(cT_edgeMP,peakVertex,p1,CT);
        Triangle* direct_neigh ;
        EdgeOrder nT_Ledge;
        getNeighTriandLedge(cT_Ledge,CT,direct_neigh,nT_Ledge);

        HasCommonLE re = hassameedges(cT_Ledge,nT_Ledge);
        if (re == HasCommonLE::same) {
       //     std::cout << "sharing the same edge " << std::endl;
            dividetwo(cT_Ledge,CT,direct_neigh,nT_Ledge);
        }

        if (re == HasCommonLE::not_same) {
         //   std::cout << "not sharing the same edge " << std::endl;
            dividethree(cT_Ledge,CT,direct_neigh,nT_Ledge);
        }


        if (count == 300 ) {
            break;
        }
        ++count;
        Input_triangles.push_back(cT_child1);
        Input_triangles.push_back(cT_child2);



    }
    //pMesh->writemesh("check9.stl");
}

void AdaptiveTriangle::dividethree( EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge) {

    Point* neigh_LedgeMP = NeighLedge.getMidPoint();
    Point* curr_LedgeMP = currLongEdge.getMidPoint();
    int nT_LedgeID = Neigh->getLongestEdgeID();

    Point* neigh_peakVertex  = Neigh->getCorners(nT_LedgeID);

    Point* fourthpoint = currLongEdge.p0;
    Point* fifththpoint = NeighLedge.p1;

    if (*currLongEdge.p0 == *neigh_peakVertex) {
        fourthpoint = currLongEdge.p1;
    }

    if (*fourthpoint == *NeighLedge.p1) {
        fifththpoint = NeighLedge.p0;
    }


    pMesh->createTriangle(curr_LedgeMP,neigh_peakVertex,neigh_LedgeMP,Neigh);
    pMesh->createTriangle(curr_LedgeMP,neigh_LedgeMP, fourthpoint,Neigh);
    pMesh->createTriangle(neigh_LedgeMP,neigh_peakVertex,fifththpoint,Neigh);

    delInfoInDS(CT);
    Triangle* second_direct_neigh ;
    EdgeOrder second_nT_Ledge;
    getNeighTriandLedge(NeighLedge,Neigh,second_direct_neigh,second_nT_Ledge);
    HasCommonLE re = hassameedges(NeighLedge,second_nT_Ledge);
    if (re == HasCommonLE::same) {
      //  std::cout << "sharing the same edge " << std::endl;
        dividetwo(NeighLedge,Neigh,second_direct_neigh,second_nT_Ledge);

    }
    else if (re == HasCommonLE::not_same) {
       // std::cout << "not sharing the same edge, second " << std::endl;
        dividethree(NeighLedge,Neigh,second_direct_neigh,second_nT_Ledge);
    }


}

void AdaptiveTriangle::dividetwo(EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge) {

    if (currLongEdge == NeighLedge) {

        Point* cT_midpoint = currLongEdge.getMidPoint();
        Point* nT_midpoint = NeighLedge.getMidPoint();
        int nT_LedgeID = Neigh->getLongestEdgeID();
        Point* n_peakvertex_ID = Neigh->getCorners(nT_LedgeID);
        Point* fourth = Neigh->getCorners(indexOrder_2(nT_LedgeID));
        Point* fifth = Neigh->getCorners(indexOrder_1(nT_LedgeID));
        pMesh->createTriangle(cT_midpoint,n_peakvertex_ID,fourth,Neigh);
        pMesh->createTriangle(cT_midpoint,fifth,n_peakvertex_ID,Neigh);
        delInfoInDS(CT);
        delInfoInDS(Neigh);

    }
}
void AdaptiveTriangle::delInfoInDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);
    upDateDS(T);
    parents.insert(T);
}

void AdaptiveTriangle::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);
    for (auto ele : TV) {

           pMesh->establishNeighofTriangle(ele);
    }
    pMesh->fillTriangleContainers(TV,alltri);
}
void AdaptiveTriangle::getNeighTriandLedge(const EdgeOrder&ed, const Triangle* CT,  Triangle* &Neigh, EdgeOrder &NeighLedge) {
    std::vector<Triangle*> adjusten_triangles;
    pMesh->getAdjustenNeigh(ed,adjusten_triangles);
    if ( adjusten_triangles.size() != 2) {
//        std::cout << *CT << std::endl;
//        std::cout << *Neigh << std::endl;
        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
        return;
    }
    for(auto ele : adjusten_triangles) {
        if (*ele == *CT ) continue;
        Neigh=ele;
    }

    int nT_LedgeID = Neigh->getLongestEdgeID();
    EdgeOrder nT_Ledge = Neigh->getEO(nT_LedgeID);
    NeighLedge = nT_Ledge;
}

HasCommonLE AdaptiveTriangle::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {
    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }
}