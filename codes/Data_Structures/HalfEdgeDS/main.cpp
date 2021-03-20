#include <iostream>
#include "HalfEdge.h"
#include "Mesh.h"
#include "InOut.h"
int main() {

//    Mesh* pMesh = new Mesh();
//    Vertex* v1 =  pMesh->createVertex(2,2,2);
//    Vertex* v2 = pMesh->createVertex(2,14,3);
//    Vertex* v3 = pMesh->createVertex(2,10,4);
//    Vertex* v4 = pMesh->createVertex(14,10,5);
//    pMesh->addVertex(v1);
//    pMesh->addVertex(v2);
//    pMesh->addVertex(v3);
//    pMesh->addVertex(v4);
//  //  pMesh->addVertex(v4);
//    pMesh->addFace(v1,v2,v4);
//    pMesh->addFace(v1,v4,v3);
//  //  pMesh->addFace(v1,v3,v2);
//  pMesh->establishconnectivity();
//pMesh->fun();
std::shared_ptr<Mesh> pMesh(new Mesh());
//pMesh = ReadSTL("/home/aravindhkumar.kalimuthu/Downloads/single_cube_holes_stl.stl");
    pMesh = ReadSTL(   "/home/aravindhkumar.kalimuthu/Downloads/Cube_3d_printing_sample.stl");

//pMesh->printdatastructinfo();
pMesh->fun();
//
//   std::cout <<  (pMesh->addVertex(v1))->get_handle() << std::endl;
//    std::cout <<  (pMesh->addVertex(v2))->get_handle() << std::endl;
//    std::cout <<  (pMesh->addVertex(v3))->get_handle() << std::endl;
//    std::cout <<  (pMesh->addVertex(v4))->get_handle() << std::endl;
//    std::cout <<  (pMesh->addVertex(v4))->get_handle() << std::endl;

}