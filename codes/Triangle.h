#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "Mesh.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Vector_3.h"
#include "Edge.h"
#include "Macros.h"


typedef Vector_3<double> Vector3;
class Mesh; //fwd




class Triangle {
public:
    Triangle();
    Triangle(Point* P0, Point* P1, Point* P2, Mesh* mobj);
    void Initilize(Point* P0, Point* P1, Point* P2);
    Point* getCorners(int i) const;
    Point* getEdgeMidPoint(int i) const;
    Point* getCentroid() const;
    EdgeOrder getEO(int i) const;
    EdgeOrder* getEdge(int i) const;
    Edge getEd(int i) const;
    Vector3 getNormalVector() const;
    double getArea() const;
    Triangle* getNeigh(int i);
    Triangle* getParent();
    bool haschild();
    int getVertexID(Point* p);
    int getTriangleID(Triangle* t);
    int getLongestEdgeID();
    EdgeOrder* getEO_pointer(int i);
    int getShortestEdgeID();
    int getPeakVertexID(const EdgeOrder& c_ed);
    int getChildSize();
    void setEdge(EdgeOrder* ed, unsigned int id);
    void setNeigh(int i, Triangle* t);
    void addChild(Triangle* t);
    void clearVchild();
    void getChildren(std::vector<Triangle*> &vchild);
    void setNewVertex(Point*p , unsigned int idx);

    bool operator == (const Triangle &rhs) const;
    bool operator != (const Triangle &rhs) const;
    bool operator < (const Triangle &rhs) const;
    friend std::ostream & operator<<(std::ostream &out, const Triangle &rhs);

public:
    bool isAlive ;
    int ID;
private:

    Point* corners[3];
    Triangle* neigb[3];
    EdgeOrder* edges[3];
    std::vector<Triangle*> vchildren;
    Mesh *powner;
    Triangle* parentT=this;
};

#endif