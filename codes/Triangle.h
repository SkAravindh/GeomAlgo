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
    Triangle* getNeigh(int i);
    Triangle* getParent();
    Edge getEd(int i) const;
    EdgeOrder getEO(int i) const;
    EdgeOrder& getfE(int i);
    Vector3 getNormalVector() const;
    bool haschild();
    bool has_Degenerated();
    bool has_contained(Point* p);
    int getShortestEdgeID();
    int getPeakVertexID(const EdgeOrder& c_ed);
    int getChildSize();
    int getVertexID(Point* p);
    int getTriangleID(Triangle* t);
    int getLongestEdgeID();
    unsigned int getThirdCorner(Point* v1, Point* v2);
    double getArea() const;
    void setEdge(EdgeOrder &ed, unsigned int id);
    void setEdge_pointer(EdgeOrder *ed, unsigned int id);
    void setNeigh(int i, Triangle* t);
    void addChild(Triangle* t);
    void clearVchild();
    void getChildren(std::vector<Triangle*> &vchild);
    void setNewVertex(Point*p , unsigned int idx);
    void flip();
    bool operator == (const Triangle &rhs) const;
    bool operator != (const Triangle &rhs) const;
    bool operator < (const Triangle &rhs) const;
    friend std::ostream & operator<<(std::ostream &out, const Triangle &rhs);

public:
    bool isAlive ;
    int ID;

public:
    bool is_set    = false;
    bool is_inside = false;

private:
    Point* corners[3];
    Triangle* neigb[3];
    EdgeOrder* edges[3];
    EdgeOrder fEdges[3];
    std::vector<Triangle*> vchildren;
    Mesh *powner;
    Triangle* parentT=this;
};

#endif