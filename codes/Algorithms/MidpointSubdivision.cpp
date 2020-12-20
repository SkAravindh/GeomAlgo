#include "MidpointSubdivision.h"


MidSubdivision::MidSubdivision (std::shared_ptr<Mesh> &Mesh) : pMesh(Mesh){
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
    Triangle* tri1  = Mesh->CreateTriangle(edge0mid,edge1mid,edge2mid);
    Triangle* tri2  = Mesh->CreateTriangle(edge0mid,parentT->getCorners(2),edge1mid);
    Triangle* tri3  = Mesh->CreateTriangle(edge1mid,parentT->getCorners(0),edge2mid);
    Triangle* tri4  = Mesh->CreateTriangle(edge2mid,parentT->getCorners(1),edge0mid);

    //Pushing triangles into current object triangle container.
    std::vector<Triangle*> thisTvec;
    thisTvec.push_back(tri1);
    thisTvec.push_back(tri2);
    thisTvec.push_back(tri3);
    thisTvec.push_back(tri4);

    //Level Increment
    ++level;

    for(TI it=thisTvec.begin(); it!=thisTvec.end(); it++){
        Vchild.push_back(new MidSubdivision(*it, level,Mesh) );
    }

}


void MidSubdivision::loadParameters(Triangle* TInput, Point* PInput , int Refinement_Level) {
    RefineLevel=Refinement_Level;
    std::vector<Triangle*> tv;
    if(PInput== nullptr){
        pMesh->getNeigTrianglesbyOrder(TInput,1,tv);
        pMesh->fillExternelTriangleVec(tv);
    }
    else{
        pMesh->getRingNeigbyOrder(PInput,2,tv);
        pMesh->fillExternelTriangleVec(tv);
    }
}

void MidSubdivision::run() {
    std::vector<Triangle*> splitvector;
    pMesh->getExternelTriangleVec(splitvector);
    for(int i=0; i<1; i++) {
        MidSubdivision *obj = new MidSubdivision(splitvector[2], 0, pMesh); //use [2] for proper result
        MidSubVector.push_back(obj);
    }
    //std::cout << *MidSubVector[0]->Vchild[0]->getParent();

}


void MidSubdivision::getSubdividedTriangles(std::vector<Triangle* > &tv) {
    for(MSI iter = MidSubVector.begin(); iter!= MidSubVector.end(); iter++){
        MidSubdivision* C_obj(*iter);
        C_obj->collectallMSDtriangles(tv);
    }

}


void MidSubdivision::collectallMSDtriangles( std::vector<Triangle *> &STV) {

    if( Vchild.empty() ){
        STV.push_back(this->getParent());
    }
    else{
        //return;
    }

    //recursively collect all subdivided triangle object.
    for(MSI iter = Vchild.begin(); iter!= Vchild.end(); iter++){
        MidSubdivision* C_vc(*iter);
        C_vc->collectallMSDtriangles(STV);
    }
}

Triangle* MidSubdivision::getParent() {
    return parentT;
}