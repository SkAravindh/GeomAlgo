#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <cmath>
#include <cfloat>
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
    Point* createVertex(  double x,   double y,   double z);
    Point* createVertex(Point &P);
    Triangle* createTriangle(Point *P0, Point *P1, Point *P2, Triangle* parent);
    void storeTriangleInfo(Triangle* T);
    void getTriangles(std::vector<Triangle*> &TV);
    void getVertices(std::vector<Point*> &vp);
    void standAlone(std::vector<Triangle*> &tv);
    void printContainersInfo();
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
    void getAdjustenNeigh(const EdgeOrder &ed,std::vector<Triangle*> &tv);
    void getAdjustenNeigh_1(const EdgeOrder &ed,std::vector<Triangle*> &tv);
    std::vector< std::pair<EdgeOrder,EdgeOrder> > updateEdgeInfo(Triangle* To_change, Point* oldVertex, Point* newVertex); //used during edge collapse operation.
    void writeMeshSTL(std::string filename);


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

protected:
    //Iterators
    typedef std::vector<Triangle*>::iterator TV_it;
    typedef std::vector<Point*>::iterator PV_it;
    typedef std::set<Triangle *>::iterator TS_it;
    typedef std::set<Point*>::iterator PS_it;
    typedef std::multimap<Point*, Triangle*,ComparePoint>::iterator MMPT;
    typedef std::multimap<EdgeOrder, Triangle *>::iterator MMET;


};


#endif