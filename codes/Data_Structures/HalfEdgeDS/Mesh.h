#ifndef MESH_H
#define MESH_H
#include <vector>
#include <map>
#include <set>
#include "HalfEdge.h"
#include <unordered_map>
#include "Common.h"

class Mesh {
public:
    Mesh() = default;
    Vertex* createVertex(double x,double y,double z);
    Vertex* addVertex(Vertex* v);
    Face* addFace(Vertex* v1,Vertex* v2,Vertex* v3);
    Edge* addEdge(Vertex* v1,Vertex* v2, Face* f);
    HalfEdge* addHalfEdge(Vertex* v1,Vertex* v2,Face* f);
    void establishconnectivity();
    void printdatastructinfo();
    void fun() {
      std::cout << "allface "<<allFaces.size() << std::endl;
        std::cout << "vertex "<<allVertex.size() << std::endl;
    }

private:
    std::unordered_map<unsigned int, Vertex*> Vertexhandle_to_Vertex_Map;
    std::unordered_map<Vertex*,unsigned int,Vertex_Hash,CompareVertex> Vertex_to_Vertexhandle_Map;

    std::unordered_map<unsigned int, Face*> Facehandle_to_Face_Map;
    std::unordered_map<Face*,unsigned int,Face_Hash, CompareFace> Face_to_Facehandle_Map;

    std::unordered_map<unsigned int, Edge*> Edgehandle_to_Edge_map;
    std::unordered_map<Edge*, unsigned int, Edge_Hash, CompareEdge> Edge_to_Edgehandle;

    std::unordered_map<unsigned int, HalfEdge*> HalfEdgehandle_to_HEdge_map;
    std::unordered_map<HalfEdge*, unsigned int, HalfEdge_Hash, CompareHalfEdge> HalfEdge_to_HalfEdgehandle;

    std::unordered_map<Face*,HalfEdge*,Face_Hash, CompareFace> Face_to_Halfedge_Map;
    std::unordered_map<Edge*,HalfEdge*,Edge_Hash, CompareEdge> Edge_to_oneHalfEdge;
private:
    std::vector<Vertex*> allVertex;
    std::vector<Face*> allFaces;
    std::vector<Edge*> allEdges;
    std::vector<HalfEdge*> allHalfEdges;
};

#endif
