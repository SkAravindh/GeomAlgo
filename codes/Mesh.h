#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include "Point.h"
#include "Triangle.h"
#include "EdgeOrder.h"
#include "Writedata.h"
#include <set>

class Triangle; //fwd

enum VecContainerName{
    alltri,
    external
};


class Mesh{
public:
    Mesh();
    Mesh(std::string Filename, size_t Poolsize);
    Point* CreateVertex(  double x,   double y,   double z);
    Point* CreateVertex(Point &P);
    Triangle* CreateTriangle(Point *P0, Point *P1, Point *P2, Triangle* parent);
    void StoreTriangleInfo(Triangle* T);
    void getTriangles(std::vector<Triangle*> &TV);
    void standAlone(std::vector<Triangle*> &tv);
    void printInfo();
    void getNeigTrianglesbyOrder(Triangle * t,  unsigned int &&order, std::vector<Triangle*> &TV);
    void getRingNeigbyOrder(Point* p, unsigned int &&order, std::vector<Triangle*> &TV);
    void fillTriangleContainers(std::vector<Triangle*> &etv,VecContainerName VN );
    void getTriangleContainers(std::vector<Triangle*> &etv,VecContainerName VN );
    void clearTV(VecContainerName VN);
    void reEstablishConnectivity();
    void delCertainTriInalltriangles(Triangle* t);
    void delCertainEntryPT(Triangle* t);
    void delCertainEntryET(Triangle* t);
    void establishNeighcompleteMesh();
    void establishNeighofTriangle(Triangle *t);
    void getAdjustenNeigh(const EdgeOrder &oe,std::vector<Triangle*> &tv);
    void writemesh(std::string filename);


private:
    std::string ModelName;
    size_t Poolsize;

protected:
    std::map<Point, Point*> allvertices;
    std::vector<Point*> vAllvertices;
    int Number_OF_Vertices=0;
    std::multimap<Point*, Triangle*,ComparePoint> mmpointTotriangles;
    std::multimap<EdgeOrder, Triangle*> mmedgeTotriangles;
    //std::multimap<EdgeOrder, std::vector<Triangle*>> map_edgeTotriangles; //using multimap map bcoz easy to acess data but not efficient
    std::vector<Triangle*> allTriangles;
    std::vector<Triangle*> externalUse;

};


#endif