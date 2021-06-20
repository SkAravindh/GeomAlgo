#ifndef QUAD_H
#define QUAD_H
#include "Mesh.h"

class Quad {
public:
    Quad() = default;
    Quad(Point* ID_0, Point* ID_1, Point* ID_2, Point* ID_3,Mesh* mobj) {
        coord_ID[0] = ID_0;
        coord_ID[1] = ID_1;
        coord_ID[2] = ID_2;
        coord_ID[3] = ID_3;
        pmesh_owner = mobj;
    }
    Point* getCoordID(unsigned int i) {
       return coord_ID[i];
    }

public:
    unsigned int ID;
private:
    Point* coord_ID[4];
    Mesh *pmesh_owner;
};

#endif