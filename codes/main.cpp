#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"
#include "Algorithms/AdaptivieTriangle.h"








int main() {
    std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/bunny_S_Up_R.stl";

    //std::string filename1 = "/home/aravindhkumar.kalimuthu/Downloads/bunny.stl";
    std::shared_ptr<Mesh> pMesh = ReadSTL(filename);
    std::vector<Triangle*> v;
    pMesh->getTriangles(v);
    pMesh->printInfo();
//    std::cout << "size  " << v.size() << std::endl;
//    std::vector<Triangle*> sh;
//    EdgeOrder ed = v[1673]->getEO(0);
//    pMesh->getAdjustenNeigh(ed,sh);
//    for(auto ele :sh){
//        std::cout << *ele << std::endl;
//    }
//    int tri1 = getNonCommonPointsIDs(sh[0],sh[1],1);
//    int tri2 = getNonCommonPointsIDs(sh[0],sh[1],2);
//    std::cout << tri1 <<" "<<tri2<<std::endl;
//    std::cout << *sh[0]->getCorners(tri1) << std::endl;
//    std::cout << *sh[1]->getCorners(tri2) << std::endl;

//    EstablishConnectivity * obj =  new EstablishConnectivity(pMesh);
//    obj->loadParameters(v[0],2);

//    std::vector<Triangle* > tv;
//    MidSubdivision * obj = new MidSubdivision(pMesh);
//    obj->loadParameters(nullptr, nullptr ,2); //allTriangles[0]->getCorners(0) point
//   obj->run();
//    obj->runForCompleteMesh();
//    obj->getSubdividedTriangles();
//    pMesh->writemesh("glob.stl");









//    std::shared_ptr<Mesh> mesh(new Mesh);
//     Point* p0 = mesh->createVertex(0,0,0);
//     Point* p1 = mesh->createVertex(10,0,0);
//     Point* p2 = mesh->createVertex(0,10,0);
//     Point *p= new Point(5,0,0);
//     Triangle* tri = new Triangle(p0,p1,p2,&*mesh);
//     bool result = brayCentric(tri,p);

//    Point* u0 = mesh->createVertex(2,2,3);
//    Point* u1 = mesh->createVertex(6,3,5);
//    Point* u2 = mesh->createVertex(4,6,7);
//    Triangle* tri1 = new Triangle(u0,u1,u2,&*mesh);







//    Mesh* pMesh = new Mesh();
//
//    Point* p0 = pMesh->createVertex(-4,3,0);
//    Point* p1 = pMesh->createVertex(4,3,0);
//    Point* p2 = pMesh->createVertex(-3,-5,4);
//    Triangle* tri = new Triangle(p0,p1,p2,&*pMesh);
//
//    Point* v0 = pMesh->createVertex(2, 2, 1);
//    Point* v1 = pMesh->createVertex(5, 2, 1);
//    Point* v2 = pMesh->createVertex(3, 6, 1);
//    Triangle* tri1 = new Triangle(v0,v1,v2,&*pMesh);
//    std::cout << tri1->getNormalVector() << std::endl;
//
//
//    Point* u0 = pMesh->createVertex(3, 2, 6);
//    Point* u1 = pMesh->createVertex(11, 2, 5);
//    Point* u2 = pMesh->createVertex(13, 10, 5);
//    Triangle* tri2 = new Triangle(u0,u1,u2,&*pMesh);
//    std::cout << tri2->getNormalVector() << std::endl;
  //bool result = TriangleTriangleIntersection(tri1,tri2);

//  Bbox_3 obj;
//  obj.add_coordinates(5,6,7);
//    obj.add_coordinates(-1,-2,-7);
//    obj.add_coordinates(10,21,5);
//
//    std::cout << obj << std::endl;
//  Bbox_3 b1(1,2,3,4,5,6);
//  Bbox_3 b2(11,12,13,14,15,16);
//  b1+b2;
//  std::cout <<b1<<std::endl;
//






}