#ifndef MESH_H
#define MESH_H
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <memory>
#include <cmath>
#include <cfloat>
#include <set>
#include <random>
#include "Point.h"
#include "Triangle.h"
#include "EdgeOrder.h"
#include "Writedata.h"
#include "Edge.h"
#include "Bbox_3.h"
#include "Rtree.h"
#include "Quad.h"

class Triangle; //fwd
class Point; //fwd
class Quad; //fwd


enum VecContainerName{
    alltri,
    external
};

enum class CentroidType {
    vertex_based, area_based, volume_based
};



class Mesh {
public:
    Mesh();
    ~Mesh();
    Mesh(std::string Filename, size_t Poolsize);
    Point* createVertex( const double x, const double y, const double z);
    Point* createVertex(const Point &P);
    Triangle* createTriangle(Point *P0, Point *P1, Point *P2, Triangle* parent);
    Quad* addQuad(Point* ID_0, Point* ID_1, Point* ID_2, Point* ID_3);
    std::vector< std::pair<EdgeOrder,EdgeOrder> > updateEdgeInfo(Triangle* To_change, Point* oldVertex, Point* newVertex); //used during edge collapse operation.
    const Rtree3d& getVertexTree() const ;
    const Rtree3d& getFaceTree() const ;
    Vector3 getCentroid(const CentroidType ct=CentroidType::vertex_based);
    Vector3 centroidVertex();
    Vector3 centroidSurface();
    Vector3 centroidVolume();
    Vector3 getVertexNormal(Point* p);
    Bbox_3 faceBounds(const Triangle* t);
    Bbox_3 getBBOX();
    double getMesh_Volume();
    double getMesh_SurfaceArea();
    size_t computeclosest(Point* p);
    bool is_Solid(std::vector<EdgeOrder>* border, std::vector<EdgeOrder>* nonmanifold);
    bool isNon_Manifold_Vertex(Point* input_vertex, std::vector<Triangle*> *ring_triangle);
    void storeTriangleInfo(Triangle* T);
    void getTriangles(std::vector<Triangle*> &TV);
    void getVertices(std::vector<Point*> &vp);
    void getVerticesAll(std::vector<Point*> &vp);
    void getQuads(std::vector<Quad*> &quad);
    void standAlone(std::vector<Triangle*> &tv);
    void printContainersInfo();
    void clear();
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
    //void establishEdgeinfo();
    void establishNeighofTriangle(Triangle *t);
    void getAdjustenNeigh(const EdgeOrder& ed, std::vector<Triangle*> &tv);
    void getAdjustenNeigh_1(const EdgeOrder& ed,std::vector<Triangle*> &tv);
    void translateMesh(const Point* p);
    void createQuadFromTriangle();
    void computeRtree();
    void writeMeshSTL(std::string filename);



private:
    std::string ModelName;
    size_t Poolsize;

private:
    std::map<Point, Point*> allvertices;
    std::vector<Point*> vAllvertices;
    int Number_OF_Vertices=0;
    std::multimap<Point*, Triangle*,ComparePoint> mmpointTotriangles;
    std::multimap<EdgeOrder, Triangle*> mmedgeTotriangles;

    //std::map<EdgeOrder, std::vector<Triangle*>> map_edgeTotriangles; //using multimap map bcoz easy to acess data but not efficient
    std::vector<Triangle*> allTriangles;
    std::vector<Triangle*> externalUse;
    std::vector<Quad*> allQuads;
    Rtree3d mFaceTree;
    Rtree3d mVertexTree;

protected:
    //Iterators
    typedef std::vector<Triangle*>::iterator TV_it;
    typedef std::vector<EdgeOrder>::iterator EV_it;
    typedef std::vector<Point*>::iterator PV_it;
    typedef std::set<Triangle *>::iterator TS_it;
    typedef std::set<Point*>::iterator PS_it;
    typedef std::multimap<Point*, Triangle*,ComparePoint>::iterator MMPT;
    typedef std::multimap<EdgeOrder, Triangle*>::iterator MMET;




};


#endif