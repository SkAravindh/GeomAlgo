#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"
//#include "verification/subdivision_EDbased.cpp"
#include "verification/adaptdesirededge.cpp"
//#include "Algorithms/SelectiveRefinement.h"






int main() {
   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunny_S_Up_R.stl";
  // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/final.stl";



    std::shared_ptr<Mesh> pMesh = ReadSTL(filename);

    DesiredEdge obj(pMesh);

    obj.do_refine();

   pMesh->writeMeshSTL("final.stl");

     //   std::vector<Triangle*> alltv;
       // pMesh->getTriangles(alltv);
//    SelectiveRefine obj(pMesh);
//    obj.loadParameter(2);
//    obj.createPatch(alltv[0], nullptr,10);
//    obj.doRefine();
//    obj.createPatch(alltv[3000], nullptr,10);
//    obj.doRefine();
//    obj.createPatch(alltv[4000], nullptr,10);
//    obj.doRefine();
//    obj.createPatch(alltv[450], nullptr,10);
//    obj.doRefine();
//    pMesh->writeMeshSTL("out.stl");



  // Refine obj(pMesh);
   //obj.doRefine(0);
//   obj.doRefine(4000);
//   obj.doRefine(8000);
//   std::vector<Triangle*> vecall;
//    pMesh->getTriangles(vecall);
//    std::cout << "size " << vecall.size() << std::endl;

//    std::vector<Triangle*> alltv;
//    pMesh->getTriangles(alltv);
//    std::vector<EdgeOrder> ed;
//    getedgesByOrder(alltv,3,ed);
//    std::cout << "size of ed " <<ed.size()<< std::endl;


//std::vector<Triangle*> alltv;
//pMesh->getTriangles(alltv);
//
//FaceMidPoint::Subdivision obj(pMesh);
//obj.loadParameter(nullptr,alltv[0]->getCorners(0),2);
//obj.run();
//pMesh->writeMeshSTL("griffdivision.stl");





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







}
