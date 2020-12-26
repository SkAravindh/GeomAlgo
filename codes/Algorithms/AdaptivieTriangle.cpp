#include "AdaptivieTriangle.h"

EstablishConnectivity::EstablishConnectivity(std::shared_ptr<Mesh> &pMesh):pMeshptr(pMesh){

}

EstablishConnectivity::EstablishConnectivity(Triangle* T, int C_level,std::shared_ptr<Mesh> &Mesh,bool flag ) {
    this->level = C_level;
    //Assigning current triangle to parent.
    parentT = T;

    //Return current triangle object as if reach the given level.
    if (level >= RefineLevel) {
        return;
    }

    Point *edge0mid = parentT->getEdgeMidPoint(0);
    Point *edge1mid = parentT->getEdgeMidPoint(1);
    Point *edge2mid = parentT->getEdgeMidPoint(2);


    //Splitting the parent triangle into four and storeing into alltriangle container(check Mesh.h)
    Triangle *tri1 = Mesh->CreateTriangle(edge0mid, edge1mid, edge2mid, parentT);
    Triangle *tri2 = Mesh->CreateTriangle(edge0mid, parentT->getCorners(2), edge1mid, parentT);
    Triangle *tri3 = Mesh->CreateTriangle(edge1mid, parentT->getCorners(0), edge2mid, parentT);
    Triangle *tri4 = Mesh->CreateTriangle(edge2mid, parentT->getCorners(1), edge0mid, parentT);
    std::vector<Triangle *> child;
    parentT->getChildren(child);

    Mesh->delCertainEntryET(parentT);
    Mesh->delCertainEntryPT(parentT);
    Mesh->delCertainTriInalltriangles(parentT);
    Mesh->fillTriangleContainers(child,alltri);

    if(flag){
        std::cout << "return  " << std::endl;
        return;
    }

    for (int i = 0; i < 3; i++) {
        std::vector<Triangle *> tv;
        Triangle *adj = parentT->getNeigh(i);
        Mesh->delCertainTriInalltriangles(adj);
        std::cout << "hi2 " << std::endl;
        if (!adj->isRefined() ) {
            std::cout << "hi1 " << std::endl;
            adj->setRefinelevel(0);
            if (!adj->haske() && !adj->getpartofke()) {
                //split for ke
                divideKE(parentT, adj, Mesh);

                adj->setke(true);
            }
            else if (parentT->haske() && adj->getpartofke()){
                std::cout << "true " << std::endl;
            }

            else {
                int rel = adj->getRefinelevel();
                rel + 1;
                adj->setRefinelevel(rel);
                adj->setRefineStatus(true);
            }

        }

    }
    ++level;

    int count =0;
    for(auto ele: child){
        ++count;
        bool onele = false;
        if(count==1){
            onele=true;
        }
        Mesh->establishNeighofTriangle(ele);
        EstablishConnectivity(ele,level,Mesh, onele);
    }

//    std::vector<Triangle *> ring;
//    ele->getChildren(newrt);
//    Mesh->getNeigTrianglesbyOrder(parentT, 1, ring);
//    for (auto ele : ring) {
//        ele->getChildren(newrt);
//        Mesh->delCertainEntryET(ele);
//        Mesh->delCertainEntryPT(ele);
//    }
//    Mesh->delCertainTrisInalltriangles(ring);
//    Mesh->fillTriangleContainers(newrt, alltri);
//    std::cout << newrt.size() << std::endl;
//    Mesh->writemesh("refine.stl");
//    writeSTL("newtri.stl", newrt);
//
//    for (auto ele : newrt) {
//
//        Mesh->establishNeighofTriangle(ele);
//    }



}


void EstablishConnectivity::loadParameters(Triangle *TInput, int Refinement_Level) {
    RefineLevel=Refinement_Level;
    pMeshptr->establishNeighcompleteMesh();
    std::vector<Triangle*> TV;
    //pMeshptr->printInfo();
    EstablishConnectivity(TInput,0,pMeshptr,false);
    pMeshptr->writemesh("che.stl");

}

void EstablishConnectivity::divideKE(Triangle *parent, Triangle *t2,std::shared_ptr<Mesh> &Mesh ) {
   int oppt2 = getNonCommonPointsIDs(parent,t2,2);
   Point *mid = t2->getEdgeMidPoint(oppt2);
   Point *p0  = t2->getCorners(indexOrder_1(oppt2));
   Point *p1  = t2->getCorners(indexOrder_2(oppt2));
   Point *O_po  = t2->getCorners(oppt2);
   Triangle* Ct1 =  Mesh->CreateTriangle(p0,mid,O_po,t2);
   Triangle* Ct2 =  Mesh->CreateTriangle(mid,p1,O_po,t2);
   Ct1->setPartofke(true);
   Ct2->setPartofke(true);
    Mesh->delCertainEntryET(t2);
    Mesh->delCertainEntryPT(t2);
    Mesh->establishNeighofTriangle(Ct1);
    Mesh->establishNeighofTriangle(Ct2);




//   EdgeOrder oe = t2->getEO(oppt2);
//   std::cout <<"by1  " << *t2->getCorners(indexOrder_1(oppt2)) << std::endl;
//   std::cout <<"by2  " <<*t2->getCorners(indexOrder_2(oppt2)) << std::endl;
//   //parent->getCorners(2);
//   //std::cout <<"mid " << *mid << std::endl;
//   std::cout <<"oe " <<oe << std::endl<<std::endl;








}