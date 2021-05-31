#ifndef EXTRACTINTTRIANGLE_H
#define EXTRACTINTTRIANGLE_H
#include "../Mesh.h"
#include "../LoopEdges.cpp"

//This algorithms requires user interaction, user has to select right loops for input.
//This algorithm calculates all loops in geometry and user interaction is requires.
//With minor changes this algorithm can be extended to automatically choose right loop but it may fail in complex geometries.
//Due to above reason, I made this algo with user interaction.

class Face {
public:
    Face() = default;
    Face(unsigned int a, unsigned int b);
    unsigned int getFID(unsigned int i);
public:
    unsigned int F_ids[2];
};

class ExtractIntTri {
public:
    ExtractIntTri() = default;
    explicit ExtractIntTri(std::shared_ptr<Mesh> &pmesh);
    void computeFeatureEdges();
    void establishConnectivity();
    unsigned int findInitialTriangle();
    void extractInteriorTriangles(unsigned int id);
    void run();

private:
    std::shared_ptr<Mesh> pMesh;
    std::map<EdgeOrder, Face> edge_to_face;
    std::vector<Triangle *> alltri;
};

#endif

