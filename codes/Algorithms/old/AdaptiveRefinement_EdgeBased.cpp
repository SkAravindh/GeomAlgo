#include "AdaptiveRefinement_EdgeBased.h"

AdaptRefineED::AdaptRefineED(std::shared_ptr<Mesh> &mesh) : pMesh(mesh) {

}

void AdaptRefineED::loadParameters(Triangle *input_Triangle, int refine_Level) {

    inputTriangle = input_Triangle;
    refineCount = refine_Level;
}

void AdaptRefineED::run() {
    std::cout << "run from adaptive refinement " << std::endl;
    edgeRefinement();
}

void AdaptRefineED::edgeRefinement() {

    //std::vector<Triangle*> trianglevector;
    //std::vector<Triangle*> ringtrianglevector;
   // pMesh->getTriangles(trianglevector);
  //  pMesh->getRingNeigbyOrder(trianglevector[0]->getCorners(0),1,ringtrianglevector);
    int count = 0;
    std::vector<Triangle*> Triangles_to_Refine;
    //Triangles_to_Refine.assign(ringtrianglevector.begin(),ringtrianglevector.end());
    Triangles_to_Refine.push_back(inputTriangle);

    while(!Triangles_to_Refine.empty()) {
        Triangle* CT = *Triangles_to_Refine.begin();
        eraseCertainTriangle(Triangles_to_Refine,CT);
        TS_it it;
        it = parents.find(CT);
        if (it != parents.end()) {
            continue;
        }

        int CurrentT_LedgeID       =  CT->getLongestEdgeID();
        EdgeOrder CurrentT_Ledge   =  CT->getEO(CurrentT_LedgeID);
        int CurrentT_peakVertexID  =  CT->getPeakVertexID(CurrentT_Ledge);
        Point* CurrentT_edgeMP     =  CurrentT_Ledge.getMidPoint();
        Point* peakVertex          =  CT->getCorners(CurrentT_peakVertexID);

        Point* p1    = CT->getCorners(indexOrder_1(CurrentT_peakVertexID));
        Point* p2    = CT->getCorners(indexOrder_2(CurrentT_peakVertexID));

        //Splitting the current trianglel;
        Triangle* CurrentT_child1 = pMesh->createTriangle(CurrentT_edgeMP,p2,peakVertex,CT);
        Triangle* CurrentT_child2 = pMesh->createTriangle(CurrentT_edgeMP,peakVertex,p1,CT);
        Triangle* direct_neigh ;
        EdgeOrder NeighT_Ledge;

        getNeighTriandLedge(CurrentT_Ledge,CT,direct_neigh,NeighT_Ledge);
        HasCommonLE re = hassameedges(CurrentT_Ledge,NeighT_Ledge);

        if (re == HasCommonLE::same) {
            splitIntoTwo(CurrentT_Ledge,CT,direct_neigh,NeighT_Ledge,true);
        }
        else if (re == HasCommonLE::not_same) {
            splitIntoThree(CurrentT_Ledge,CT,direct_neigh,NeighT_Ledge, true);
        }

        if (count == refineCount ) {
            break;
        }
        ++count;

        Triangles_to_Refine.push_back(CurrentT_child1);
        Triangles_to_Refine.push_back(CurrentT_child2);

    }
}

void AdaptRefineED::splitIntoTwo(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag) {

    if (currLongEdge != NeighLedge) {
        std::cout << "Error at SplitIntoTwo function" << std::endl; //should not happen.
        return;
    }

    Point* CurrentT_midpoint    =  currLongEdge.getMidPoint();
    Point* nT_midpoint          =  NeighLedge.getMidPoint();
    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();
    Point* NeighT_peakvertex_ID =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* F_5 = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(CurrentT_midpoint,NeighT_peakvertex_ID,F_4,Neigh);
    pMesh->createTriangle(CurrentT_midpoint,F_5,NeighT_peakvertex_ID,Neigh);
    if (flag) {
        deleteInfoFromDS(CT);
    }
    deleteInfoFromDS(Neigh);

}

void AdaptRefineED::splitIntoThree(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag) {

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

    if(flag) {
        deleteInfoFromDS(CT);
    }

    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge);
    HasCommonLE re = hassameedges(NeighLedge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge, true);
    }
    else if (re == HasCommonLE::not_same) {

        if (std::count(BED.begin(), BED.end(), second_NeighT_Ledge)) {
            avoidCOllision(NeighLedge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
        }
        else {
            splitIntoThree(NeighLedge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
        }
    }

}

void AdaptRefineED::avoidCOllision(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag) {

    Point* CurrentT_midpoint    =  currLongEdge.getMidPoint();
    int peak    = Neigh->getPeakVertexID(currLongEdge);
    Point* vert = Neigh->getCorners(peak);
    Point* F_4  = Neigh->getCorners(indexOrder_2(peak));
    Point* F_5  = Neigh->getCorners(indexOrder_1(peak));
    pMesh->createTriangle(CurrentT_midpoint,vert,F_4,Neigh);
    pMesh->createTriangle(CurrentT_midpoint,F_5,vert,Neigh);
    if (flag) {
        deleteInfoFromDS(CT);
    }
    deleteInfoFromDS(Neigh);

}

void AdaptRefineED::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh,
                                        EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh(ed,adjacent_triangles);

    if ( adjacent_triangles.size() != 2) {
        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
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


HasCommonLE AdaptRefineED::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void AdaptRefineED::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);
    upDateDS(T);
    parents.insert(T);

}

void AdaptRefineED::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);

    for (auto ele : TV) {
     //   pMesh->establishNeighofTriangle(ele);
    }
    pMesh->fillTriangleContainers(TV,alltri);

}