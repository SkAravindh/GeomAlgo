#ifndef HOLEDETECTION_H
#define HOLEDETECTION_H

#include <iostream>
#include <math.h>
#include <fstream>
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"

#define PI 3.14
#define Gaussian_Curvature 2*PI
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh;
typedef MyMesh::VertexHandle VertexHandle;
typedef MyMesh::HalfedgeHandle HalfedgeHandle;
typedef MyMesh::EdgeHandle EdgeHandle;
typedef MyMesh::FaceHandle FaceHandle;
typedef MyMesh::VertexOHalfedgeIter VertexOHalfedgeIter;
typedef MyMesh::VertexFaceIter VertexFaceIter;
typedef MyMesh::Point point;
typedef OpenMesh::Vec3d  Vec3d;

double getDistance(const point &p1, const point &p2);
double getAngleBtwVectors(const point &p1, const point &p2);
void writePoints(std::string filename, std::vector<point> &bp);
void getNextVertexHandle(const MyMesh &mesh, const EdgeHandle &Input_Edge, const VertexHandle &input, VertexHandle &out);
class Looping; //fwd

class HoleDetection {
public:
    HoleDetection() = default;
    explicit HoleDetection(const std::string &filename);
    void computeHoles();
    double computeGaussianCurvature(const VertexHandle &Vertex);
    double calculateArea(std::vector<VertexHandle> &vertex_ids);
    void removeDuplicates(std::vector<EdgeHandle> &possible_edges_1);
    bool getNextDuplicateEdge(VertexHandle &curent, EdgeHandle &Initial, VertexHandle &next_vertex, EdgeHandle &next_edge,std::multimap<VertexHandle,EdgeHandle> &vertex_to_Edge);
    void createLoop(std::set<HalfedgeHandle> &possible_edges_1);
    bool getNextHE(HalfedgeHandle &curr_he, HalfedgeHandle &next_he, std::multimap<VertexHandle,HalfedgeHandle> &vertex_to_edge);

private:
    MyMesh mesh;
    std::set<EdgeHandle> duplicateedges;
};

class Looping {
public:
    Looping()=default;
    void addEdge(HalfedgeHandle &he);
    bool isclosed(const MyMesh &mesh);
public:
    std::vector< HalfedgeHandle > he_vector;
};


#endif