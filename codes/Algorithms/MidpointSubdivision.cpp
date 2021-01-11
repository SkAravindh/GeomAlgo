#include "MidpointSubdivision.h"


MidSubdivision::MidSubdivision (std::shared_ptr<Mesh> &Mesh) : pMesh(Mesh) {
}

MidSubdivision::MidSubdivision(Triangle *T, int C_level,std::shared_ptr<Mesh> &Mesh) {

    this->level=C_level;

    //Assigning current triangle to parent.
    parentT = T;

    //Return current triangle object as if reach the given level.
    if(level >= RefineLevel){
        return;
    }

    //Taking Midpoint of Current Triangle to initiate the splitting process.
    Point* edge0mid = parentT->getEdgeMidPoint(0);
    Point* edge1mid = parentT->getEdgeMidPoint(1);
    Point* edge2mid = parentT->getEdgeMidPoint(2);


    //Splitting the parent triangle into four and storeing into alltriangle container(check Mesh.h)
     Mesh->createTriangle(edge0mid,edge1mid,edge2mid,parentT);
     Mesh->createTriangle(edge0mid,parentT->getCorners(2),edge1mid, parentT);
     Mesh->createTriangle(edge1mid,parentT->getCorners(0),edge2mid, parentT);
     Mesh->createTriangle(edge2mid,parentT->getCorners(1),edge0mid, parentT);

    //Storing Triangle for further subdivision.
     std::vector<Triangle*> thisTvec;
     parentT->getChildren(thisTvec);

    //Level Increment
    ++level;

    for(TI it=thisTvec.begin(); it!=thisTvec.end(); it++) {
        MidSubdivision* subdi = new MidSubdivision(*it, level,Mesh);
    }
}

void MidSubdivision::loadParameters(Triangle* TInput, Point* PInput , int Refinement_Level) {

    RefineLevel=Refinement_Level;
    std::vector<Triangle*> tv;
    if(TInput != nullptr) {
        pMesh->getNeigTrianglesbyOrder(TInput,1,tv);
        pMesh->fillTriangleContainers(tv, external);
    }
    else if(PInput != nullptr) {
        pMesh->getRingNeigbyOrder(PInput,1,tv);
        pMesh->fillTriangleContainers(tv, external);
    }
    else {
        std::cout << "Midpoint Subdivision Initialized for complete mesh " << std::endl;
        forCompleteMesh = true;
    }
}

void MidSubdivision::run() {

    if (forCompleteMesh) {
        runForCompleteMesh();
        forCompleteMesh = false;
        return;
    }

    pMesh->getTriangleContainers(TV,external);
    for(auto ele : TV) {
        pMesh->delCertainTriInalltriangles(ele);
    }

    for(int i=0; i<TV.size(); i++) {
        MidSubdivision *obj = new MidSubdivision(TV[i], 0, pMesh); //use [2] for proper result
    }

}

void MidSubdivision::runForCompleteMesh() {

    pMesh->getTriangles(TV);
    pMesh->clearTV(alltri);

    for(int i=0; i<TV.size(); i++) {
        MidSubdivision *obj = new MidSubdivision(TV[i], 0, pMesh); //use [2] for proper result
    }

}

void MidSubdivision::getSubdividedTriangles() {

    std::vector<Triangle *> out;
    for (auto ele : TV) {
        ele->getChildren(out);
    }

    pMesh->fillTriangleContainers(out, alltri);
    pMesh->reEstablishConnectivity();
    TV.clear();

}

//void MidSubdivision::getSubdividedTriangles() {

//    std::vector<Triangle*> out;
//    for(MSI iter = MidSubVector.begin(); iter!= MidSubVector.end(); iter++){
//        MidSubdivision* C_obj(*iter);
//        C_obj->collectallMSDtriangles(out);
//    }
//    std::cout << "out "<<out.size() << std::endl;

//}

//void MidSubdivision::collectallMSDtriangles( std::vector<Triangle *> &STV) {
//
//    if( Vchild.empty() ) {
//        STV.push_back(this->getParent());
//    }
//    else{
//        //return;
//    }
//
//    //recursively collect all subdivided triangle object.
//    for(MSI iter = Vchild.begin(); iter!= Vchild.end(); iter++) {
//        MidSubdivision* C_vc(*iter);
//        C_vc->collectallMSDtriangles(STV);
//    }
//}

//Triangle* MidSubdivision::getParent() {
//    return parentT;
//}