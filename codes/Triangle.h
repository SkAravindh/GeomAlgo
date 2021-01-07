#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Mesh.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Vector_3.h"
#include "Edge.h"
typedef Vector_3<double> Vector3;

class Mesh; //fwd

class Triangle{
public:
    Triangle();
    Triangle(Point* P0, Point* P1, Point* P2, Mesh* mobj);
    void Initilize(Point* P0, Point* P1, Point* P2);
    Point* getCorners(int i) const;
    EdgeOrder getEO(int i) const;
    Edge getEd(int i) const;
    Vector3 getNormalVector() const;
    Point* getEdgeMidPoint(int i) const;
    void setNeigh(int i, Triangle* t);
    Triangle* getNeigh(int i);
    void addChild(Triangle* t);
    void clearVchild();
    bool haschild();
    void getChildren(std::vector<Triangle*> &vchild);
    int getVertexID(Point* p);
    void setNewVertex(Point*p , int idx);
    int getLongestEdgeID()  ;
    int getPeakVertexID(const EdgeOrder &c_ed);


    bool operator == (const Triangle &rhs) const;
    bool operator != (const Triangle &rhs) const;
    bool operator < (const Triangle &rhs) const;
    friend std::ostream & operator<<(std::ostream &out, const Triangle &rhs);


private:
    Point* corners[3];
    Triangle* neigb[3];
    std::vector<Triangle*> vchildren;
    Mesh *powner;
    Triangle* glob=nullptr;

};

#endif