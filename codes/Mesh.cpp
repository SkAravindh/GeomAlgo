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
