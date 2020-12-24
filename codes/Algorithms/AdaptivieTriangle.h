#ifndef ADAPTIVIETRIANGLE_H
#define ADAPTIVIETRIANGLE_H
#include <iostream>
#include "../Mesh.h"
#include "../Writedata.h"


static int RefineLevel;
class EstablishConnectivity{
public:
    explicit EstablishConnectivity(std::shared_ptr<Mesh> &pMesh);
    EstablishConnectivity(Triangle* T, int C_level,std::shared_ptr<Mesh> &Mesh );
    void loadParameters( Triangle* TInput , int Refinement_Level);
    void divideKE(Triangle* t1, Triangle* t2,std::shared_ptr<Mesh> &Mesh );


private:
    std::shared_ptr<Mesh> pMeshptr;
    int level;
    Triangle* parentT;

protected:
    //Iterators;
    typedef std::vector<Triangle*>::iterator TI;
};

#endif