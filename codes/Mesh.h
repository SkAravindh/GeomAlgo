#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include "Point.h"
#include "Triangle.h"
#include "EdgeOrder.h"
class Triangle; //fwd

class Mesh{
public:
    Mesh();
    Mesh(std::string Filename, size_t Poolsize);
    Point* CreateVertex(  double x,   double y,   double z);
    Point* CreateVertex(Point &P);
    void CreateTriangle(Point *P0, Point *P1, Point *P2);
    void StoreTriangleInfo(Triangle* T);
    void getTriangles(std::vector<Triangle*> &TV);



private:
    std::string ModelName;
    size_t Poolsize;


protected:
    std::map<Point, Point*> allvertices;
    std::vector<Point*> vAllvertices;
    int Number_OF_Vertices=0;
    std::multimap<Point*, Triangle*,ComparePoint> mmpointTotriangles;
    std::multimap<EdgeOrder, Triangle*> mmedgeTotriangles;
    std::vector<Triangle*> allTriangles;

};


#endif