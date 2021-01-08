#include "LaplacianSmoothing.h"

LaplacianSmooth::LaplacianSmooth(std::shared_ptr<Mesh> &mesh) : pMesh(mesh) {
    std::cout << "This smoothing algorithm would work properly if given mesh is MANIFOLD " << std::endl;
}

void LaplacianSmooth::loadParameters(int No_of_Iteration) {
    Iteration_count = No_of_Iteration;
}

void LaplacianSmooth::run() {
    smoothMesh();
}

void LaplacianSmooth::smoothMesh() {

    std::map<Point*, Point*,ComparePoint>::iterator it;
    for(int i=0; i<Iteration_count; i++) {
        std::cout << "Iteration : " << i << std::endl;
        std::vector<Point*> allPoints;
        pMesh->getVertices(allPoints);

        for(auto current_point : allPoints) {
            std::vector<Triangle*> Neigh_Triangle;
            std::vector<EdgeOrder> Boader_Edge;
            std::vector<Point*> Neigh_Points;
            Point* cp(current_point);
           // std::cout << "cp " << *cp << std::endl;

            pMesh->getRingNeigbyOrder(cp,1,Neigh_Triangle);
            getedgesByOrder(Neigh_Triangle,-1,Boader_Edge);
            getBorderPoints(Boader_Edge,Neigh_Points);
            setupToEstimateWeights(current_point,Neigh_Points);
        }
        for(it = oldVertex_to_newVertex.begin(); it != oldVertex_to_newVertex.end(); it++) {
            replaceOldvertex(it->first,it->second);
        }
        pMesh->reEstablishConnectivity();
    }

  //  pMesh->writeMeshSTL("smooth.stl");

}

void LaplacianSmooth::setupToEstimateWeights(Point* original, std::vector<Point*> &neigh_points) {

    std::map<Point*, double,ComparePoint> vertex_to_cotan;
    std::vector<Point*>::iterator it;
    for(it = neigh_points.begin(); it != neigh_points.end(); it++) {
        Point* jth(*it);
        EdgeOrder common_edge(original, jth);
        std::vector<Triangle*> Adj_Triangle;
        std::vector<int> ids;
        pMesh->getAdjustenNeigh(common_edge,Adj_Triangle);
        Triangle* t1 = Adj_Triangle[0];
        Triangle* t2 = Adj_Triangle[1];
        getNonCommonPointsIDs(t1,t2,-1,ids);
        Point* T1ncp = t1->getCorners(ids[0]);
        Point* T2ncp = t2->getCorners(ids[1]);
        double wij = coTangent(T1ncp,T2ncp,common_edge);
        vertex_to_cotan[jth] = wij;
    }
    computeNewVertex(vertex_to_cotan,original);

}

double LaplacianSmooth::coTangent(Point* p1, Point* p2, EdgeOrder c_ed) {

    Vector3 common_p1( c_ed.p0->x(),c_ed.p0->y(),c_ed.p0->z() );
    Vector3 common_p2( c_ed.p1->x(),c_ed.p1->y(),c_ed.p1->z() );

    Vector3 T1ncp(p1->x(),p1->y(),p1->z());
    Vector3 T2ncp(p2->x(),p2->y(),p2->z());

    Vector3 alpha_I = common_p1 - T1ncp;
    Vector3 alpha_J = common_p2 - T1ncp;
    double alpha_IJ = getAngleBtwVectors(alpha_I,alpha_J);
    double cotalpha =  ( cos(alpha_IJ) / sin(alpha_IJ) );

    Vector3 beta_I = common_p1 - T2ncp;
    Vector3 beta_J = common_p2 - T2ncp;
    double beta_IJ = getAngleBtwVectors(beta_I,beta_J); //returns by radians
    double cotbeta =  ( cos(beta_IJ) / sin(beta_IJ) );

    double wij = ( (cotalpha + cotbeta  )* 0.5);
    return wij;

}

void LaplacianSmooth::computeNewVertex(std::map<Point*, double,ComparePoint> &vertex_to_cotan_map, Point* origin) {

    Vector3 sum;
    double weightsum=0;
    std::map<Point*, double,ComparePoint>::iterator it;
    for(it = vertex_to_cotan_map.begin(); it != vertex_to_cotan_map.end(); it++) {
       Point* p = it->first;
       Vector3 p_vec(p->x(),p->y(),p->z());
       weightsum = weightsum + it->second;
       p_vec*it->second;
       sum+p_vec;
    }
    Point* newpoint = new Point (sum.x()/weightsum,sum.y()/weightsum ,sum.z()/weightsum );
    oldVertex_to_newVertex.insert(std::make_pair(origin,newpoint));
}

void LaplacianSmooth::replaceOldvertex(Point* oldVertex, Point* newVertex) {

    std::vector<Triangle*>::iterator it;
    std::vector<Triangle*> Ring_Neigh;
    pMesh->getRingNeigbyOrder(oldVertex,1,Ring_Neigh);
    for(it = Ring_Neigh.begin(); it != Ring_Neigh.end(); it++) {
        Triangle* t(*it);
        int current_id = t->getVertexID(oldVertex);
        t->setNewVertex(newVertex,current_id);
    }
}

