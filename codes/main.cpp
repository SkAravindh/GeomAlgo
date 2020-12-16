#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"





int main() {
    std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/bunny_S_Up_R.stl";
   // std::string filename1 = "/home/aravindhkumar.kalimuthu/Downloads/bunny.stl";
    std::shared_ptr<Mesh> pMesh = ReadSTL(filename);




    std::shared_ptr<Mesh> mesh(new Mesh);
     Point* p0 = mesh->CreateVertex(0,0,0);
     Point* p1 = mesh->CreateVertex(10,0,0);
     Point* p2 = mesh->CreateVertex(0,10,0);
     Point *p= new Point(5,0,0);
     Triangle* tri = new Triangle(p0,p1,p2,&*mesh);
     bool result = brayCentric(tri,p);
//    std::vector<Triangle*> tv;
//    pMesh->getTriangles(tv);
//    std::vector<EdgeOrder> ed;
//    edgesByOrder(tv,1,ed);
//    std::cout<<"size of ed "<<ed.size()<<std::endl;
//    writePoint(ed);
    Point* u0 = mesh->CreateVertex(2,2,3);
    Point* u1 = mesh->CreateVertex(6,3,5);
    Point* u2 = mesh->CreateVertex(4,6,7);
    Triangle* tri1 = new Triangle(u0,u1,u2,&*mesh);






//    Mesh* pMesh = new Mesh();
//
//    Point* p0 = pMesh->CreateVertex(-4,3,0);
//    Point* p1 = pMesh->CreateVertex(4,3,0);
//    Point* p2 = pMesh->CreateVertex(-3,-5,4);
//    Triangle* tri = new Triangle(p0,p1,p2,&*pMesh);
//
//    Point* v0 = pMesh->CreateVertex(2, 2, 1);
//    Point* v1 = pMesh->CreateVertex(5, 2, 1);
//    Point* v2 = pMesh->CreateVertex(3, 6, 1);
//    Triangle* tri1 = new Triangle(v0,v1,v2,&*pMesh);
//    std::cout << tri1->getNormalVector() << std::endl;
//
//
//    Point* u0 = pMesh->CreateVertex(3, 2, 6);
//    Point* u1 = pMesh->CreateVertex(11, 2, 5);
//    Point* u2 = pMesh->CreateVertex(13, 10, 5);
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



////    vector3 origin(1,4,1);  //parallel
////    vector3 rayvector(3,7,1);
    vector3 origin(1,-2,0);
    vector3 rayvector(-2,6,2);
    vector3 intersect;
    vector3 intersect1;
//  bool inter = MTRayTriangleIntersection(origin, rayvector, tri, intersect );
//  bool internew =  RayTriangleIntersection(origin, rayvector, tri, intersect1 );
////   std::cout<<inter<<std::endl;
//   if(inter){
//       std::cout << "Intersects is "<<intersect << std::endl;
//   }
//   else{
//       std::cout <<"nooooo " << std::endl;
//   }
//




}