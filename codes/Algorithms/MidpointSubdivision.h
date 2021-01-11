#ifndef MIDPOINTSUBDIVISION_H
#define MMIDPOINTSUBDIVISION_H
#include <iostream>
#include "../Mesh.h"
#include "../Geom_Support.h"
#include "../Writedata.h"

static int RefineLevel;
static std::vector<Triangle*> TV;
static bool forCompleteMesh=false;

class MidSubdivision  {
public:
    explicit MidSubdivision (std::shared_ptr<Mesh> &Mesh);
    MidSubdivision(Triangle* T, int C_level,std::shared_ptr<Mesh> &Mesh );
    void loadParameters( Triangle* TInput, Point* PInput , int Refinement_Level);
    void run();
    void getSubdividedTriangles();
   // void collectallMSDtriangles( std::vector<Triangle *> &STV);
   // Triangle* getParent();
    void runForCompleteMesh();

private:
    std::shared_ptr<Mesh> pMesh;
    int level;
    Triangle* parentT = nullptr;
    std::vector<MidSubdivision*> Vchild;
    std::vector<MidSubdivision*> MidSubVector;

protected:
    //Iterators;
    typedef std::vector<Triangle*>::iterator TI;
    typedef std::vector<MidSubdivision*>::iterator MSI;
};

#endif