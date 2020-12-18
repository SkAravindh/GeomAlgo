#include "MidpointSubdivision.h"


MidSubdivision::MidSubdivision (std::shared_ptr<Mesh> &Mesh) : pMesh(Mesh){
}

MidSubdivision::MidSubdivision(Triangle *T, int C_level,std::shared_ptr<Mesh> &Mesh) {

    //Return current triangle object as if reach the given level.
    this->level=C_level;
   
    if(level >= RefineLevel){
        return;
    }
    //Assigning current triangle to parent.
    parentT = T;

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
    this->thisTvec.push_back(tri1);
    this->thisTvec.push_back(tri2);
    this->thisTvec.push_back(tri3);
    this->thisTvec.push_back(tri4);

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
    MidSubdivision *obj = new MidSubdivision(splitvector[2],0,pMesh);
    std::vector<Triangle*> patch;
    for( auto ele :  obj->thisTvec){
        patch.push_back(ele);
    }
    for(int i=0; i<4; i++){
       for(auto ele1 : obj->Vchild[i]->thisTvec){
           patch.push_back(ele1);
       }
    }
    writeSTL("dividetri.stl", patch);
}



