#ifndef LAPLACIANSMOOTHING_H
#define LAPLACIANSMOOTHING_H
#include "../Mesh.h"
#include "../Geom_Support.h"

class LaplacianSmooth{
public:
    LaplacianSmooth()=default;
    explicit LaplacianSmooth(std::shared_ptr<Mesh> &mesh);
    void loadParameters(int No_of_Iteration);
    void run();
    void smoothMesh();
    void setupToEstimateWeights(Point* original, std::vector<Point*> &neigh_points);
    double coTangent(Point* p1, Point* p2, EdgeOrder c_ed);
    void computeNewVertex(std::map<Point*, double,ComparePoint> &vertex_to_cotan_map, Point* origin);
    void replaceOldvertex(Point* oldVertex, Point* newVertex);



private:
    std::shared_ptr<Mesh> pMesh;
    int Iteration_count=1;
    std::map<Point*, Point*,ComparePoint> oldVertex_to_newVertex;
};


#endif