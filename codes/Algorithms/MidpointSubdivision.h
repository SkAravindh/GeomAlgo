#ifndef MIDPOINTSUBDIVISION_H
#define MMIDPOINTSUBDIVISION_H
#include <iostream>
#include "../Mesh.h"
#include "../Geom_Support.h"
#include "../Writedata.h"

static int RefineLevel;
class MidSubdivision  {
public:
    explicit MidSubdivision (std::shared_ptr<Mesh> &Mesh);
    MidSubdivision(Triangle* T, int C_level,std::shared_ptr<Mesh> &Mesh );
    void loadParameters( Triangle* TInput, Point* PInput , int Refinement_Level);
    void run();



private:
    std::shared_ptr<Mesh> pMesh;
    int level;
    Triangle* parentT = nullptr;
    std::vector<Triangle*> thisTvec;
    std::vector<MidSubdivision*> Vchild;

protected:
    //Iterators;
    typedef std::vector<Triangle*>::iterator TI;
};

#endif