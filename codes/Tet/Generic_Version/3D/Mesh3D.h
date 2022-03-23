#ifndef MESH3D_H
#define MESH3D_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "Element.h"
#include "Point3D.h"
#include "Edge3D.h"
#include "Face3D.h"



typedef Face3D<Point_3* , Element*> Face_3;

class Mesh3D
{
public:
    Mesh3D();
    explicit Mesh3D(const std::string &meshName_);
    ~Mesh3D() noexcept;
    Point_3* createPoint_3(const Point_3 &p);
    Point_3* createPoint_3(const double x, const double y, const double z);
    Element* createTetrahedron(Point_3* p0,Point_3* p1,Point_3* p2,Point_3* p3);
    Element* createTetrahedron(Point_3* tetPoints[4], size_t tetIds[4]);
    Element* createTriangle(Point_3* p0,Point_3* p1,Point_3* p2);
    Element* createTriangle(Point_3* triPoints[3], size_t triIds[3]);
    const std::vector<Point_3*>& getallVertices() const;
    const std::map<Point_3*, size_t,ComparePoint3D<double,3>>& getmPoint3toID() const;
    void registerPoint_3(Element* element_);
    void registerEdge(Edge_3 obj, Element* element_);
    void registerFace(Face_3* obj, Element* element_);
    void registerEdgetoFace(Edge_3 edge, Face_3* obj);
    void getalivePolygons(std::vector<Element*> & vec);
    void getBorderFaces();
    void kill();
    void printContainerInfo();

private:
    const std::string meshName;
    std::map<Point_3, Point_3*> mPoint3toPoint3;
    std::map<Point_3*, size_t,ComparePoint3D<double,3>> mPoint3toID;
    std::map<Face_3,Face_3*> mHfacetoHface;

    //All Containers.
    std::vector<Point_3*> vAllVertices;
    std::vector<Element*> allPolygons;
    std::vector<Face_3*> allHalfFaces;
    std::multimap<Point_3*, Element* , ComparePoint3D<double,3>> mmPointtoElements;
    std::unordered_multimap<Edge_3 ,Element*,HashIndex<Point_3*,2>> ummEdgetoElements;
    std::unordered_multimap<Edge_3 ,Face_3*, HashIndex<Point_3*,2>> ummEdgetoFace;
    std::multimap<Face_3, Element*> mmFacetoElement;
private:
   size_t tetCount  = 0;
   size_t triCount  = 0;
};
#endif