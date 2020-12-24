#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Mesh.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Vector_3.h"
typedef Vector_3<double> Vector3;

class Mesh; //fwd

class Triangle{
public:
    Triangle();
    Triangle(Point* P0, Point* P1, Point* P2, Mesh* mobj);
    void Initilize(Point* P0, Point* P1, Point* P2);
    Point* getCorners(int i) const;
    EdgeOrder getEO(int i) const;
    Vector3 getNormalVector() const;
    Point* getEdgeMidPoint(int i) const;
    void setNeigh(int i, Triangle* t);
    Triangle* getNeigh(int i);
    void addchild(Triangle* t);
    void clearVchild();
    bool haschild();
    void getChildren(std::vector<Triangle*> &vchild);

    void setRefineStatus(bool flag);
    void setRefinelevel(int a);
    int getRefinelevel() const;
    bool isRefined() const;
    bool haske() const;
    void setke(bool flag);

    bool operator == (const Triangle &rhs) const;
    bool operator != (const Triangle &rhs) const;
    bool operator < (const Triangle &rhs) const;
    friend std::ostream & operator<<(std::ostream &out, const Triangle &rhs);


private:
    Point* corners[3];
    Triangle* neigb[3];
    std::vector<Triangle*> vchildren;
    Mesh *powner;
    bool refineStatus=false;
    int refineLevel;
    bool Ke = false;

};

#endif