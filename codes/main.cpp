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
    std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/single_cube_holes_stl.stl";

    std::shared_ptr<Mesh> pMesh = ReadSTL(filename);

    std::shared_ptr<Mesh> mesh(new Mesh);
    Point* p0 = mesh->CreateVertex(0,0,0);
     Point* p1 = mesh->CreateVertex(10,0,0);
     Point* p2 = mesh->CreateVertex(0,10,0);
     Point *p= new Point(5,0,0);
    Triangle* tri = new Triangle(p0,p1,p2,&*mesh);
    bool result = brayCentric(tri,p);
    std::vector<Triangle*> tv;
    pMesh->getTriangles(tv);
    std::vector<EdgeOrder> ed;
    edgesByOrder(tv,1,ed);
    std::cout<<"size of ed "<<ed.size()<<std::endl;
    writePoint(ed);
    std::cout << std::endl;


 



}
