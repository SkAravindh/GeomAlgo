#ifndef CONVEXHULL_3D_H
#define CONVEXHULL_3D_H
#include <unordered_set>
#include <unordered_map>
#include "../Mesh.h"

static constexpr float PLANE_DISTANCE = 1e-10;

template<typename T>
struct Hash {
    T operator() (const T&v) const {
        return v;
    }
};

class ConvexHull {
public:
    ConvexHull() = default;
    ConvexHull(std::vector<Point*> &Pvec);
    void compute();
    void initilize0utpoints();
    void createInitialSimplex();
    bool faceVisible(Triangle* t, Vector3 &vec);
    float facePlaneDistance(Triangle* t, Vector3 &vec);
    bool getFurtherPoint(Triangle* t, Vector3 & fp);
    void getEdgesandFaces(Triangle* t,EdgeOrder ed[3], Triangle* adj[3]);
    void updateExterior(std::vector<Triangle*> &popedfaces,std::vector<Triangle*> &newfaces);

private:
    std::vector<Point*> Input_points;
    std::unordered_set<int, Hash<size_t>, std::equal_to<size_t>> outside_points;
    unsigned int No_of_points;
    Vector3 centre;
    std::shared_ptr<Mesh> pMesh;
};


#endif