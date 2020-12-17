#include "Mesh.h"


Mesh::Mesh() {

}

Mesh::Mesh(std::string Filename, size_t Poolsize) : ModelName(Filename), Poolsize(Poolsize) {

}

Point* Mesh::CreateVertex(  double x,   double y,   double z) {
    Point P(x,y,z);
    Point * Pptr = CreateVertex(P);
    return Pptr;
}

Point* Mesh::CreateVertex(Point &P) {

    std::map<Point, Point*>::iterator it=allvertices.find(P);
    if(it != this->allvertices.end()){
        return it->second;
    }
    else{
        Point * pobj = new Point(P.x(),P.y(),P.z());
        this->allvertices.insert(std::make_pair(*pobj,pobj));
        vAllvertices.push_back(pobj);
        ++Number_OF_Vertices;
        return pobj;
    }
}

void Mesh::CreateTriangle(Point *P0, Point *P1, Point *P2) {
    Triangle *newT = new Triangle(P0, P1, P2,this);
    allTriangles.push_back(newT);

}

void Mesh::StoreTriangleInfo(Triangle *T) {
    Point* p0 = T->getCorners(0);
    Point* p1 = T->getCorners(1);
    Point* p2 = T->getCorners(2);

    mmpointTotriangles.insert(std::make_pair(p0,T));
    mmpointTotriangles.insert(std::make_pair(p1,T));
    mmpointTotriangles.insert(std::make_pair(p2,T));

    mmedgeTotriangles.insert(std::make_pair(T->getEO(0),T));
    mmedgeTotriangles.insert(std::make_pair(T->getEO(1),T));
    mmedgeTotriangles.insert(std::make_pair(T->getEO(2),T));


}

void Mesh::getTriangles(std::vector<Triangle*> &TV){
    std::vector<Triangle*>::iterator it;
    for(it=allTriangles.begin(); it!=allTriangles.end(); it++){
        Triangle * tri(*it);
        TV.push_back(tri);
    }
}

void Mesh::getNeigTrianglesbyOrder(Triangle * t,  unsigned int &&order, std::vector<Triangle*> &TV) {
    //iterators
    std::set<Triangle *>::iterator it, itt;
    typedef std::multimap<EdgeOrder, Triangle *>::iterator MMET;
    //Required Containers.
    std::set<Triangle *> visted;
    std::set<Triangle *> collected_triangles;
    std::set<Triangle *> temporary_triangles;
    collected_triangles.insert(t);

    for (int i = 0; i < order; i++) {
        temporary_triangles.clear();
        for (it = collected_triangles.begin(); it != collected_triangles.end(); it++) {

            itt = visted.find(*it);
            if (itt != visted.end()) continue;
            visted.insert(*it);

            Triangle *current_triangle(*it);
            for (int j = 0; j < 3; j++) {
                std::vector<Triangle *> Vectemp;
                EdgeOrder ed = current_triangle->getEO(j);
                std::pair<MMET, MMET> triangle_set = mmedgeTotriangles.equal_range(ed);
                if (std::distance(triangle_set.first, triangle_set.second) != 2) continue;

                for (MMET triangle_set_IT = triangle_set.first; triangle_set_IT != triangle_set.second; triangle_set_IT++) {
                    Triangle *tri = triangle_set_IT->second;
                    if (*tri == **it) continue;
                    Vectemp.push_back(triangle_set_IT->second);
                }
                if (Vectemp.size() == 1) {
                    temporary_triangles.insert(Vectemp[0]);
                }
            }
        }

//        for (auto InT = collected_triangles.begin(); InT != collected_triangles.end(); InT++) {
//            temporary_triangles.erase(*InT);
//        }
        for (auto newT = temporary_triangles.begin(); newT != temporary_triangles.end(); newT++) {
            collected_triangles.insert(*newT);
        }
    }
    TV.assign(collected_triangles.begin(), collected_triangles.end());
   // std::cout << "size of collected_triangles "<<collected_triangles.size()<<" "<<"vector_size "<<TV.size()<<std::endl;

}

void Mesh::getRingNeigbyOrder(Point *p, unsigned int &&order, std::set<Triangle *> &TS) {

}


void Mesh::standAlone() {

}