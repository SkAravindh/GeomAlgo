#include <iostream>
#include "Definition.h"
#include <memory>
#include <set>
#include "Mesh3D.h"
#include "TetIO.h"
#include "Face3D.h"


class point{
public:
    point();
    point(double x, double y, double z, size_t id){
        corners.push_back(x);
        corners.push_back(y);
        corners.push_back(z);
    }
    point(double x, double y, double z, double u){
        corners.push_back(x);
        corners.push_back(y);
        corners.push_back(z);
        corners.push_back(u);
    }
    bool operator < (const point & rhs) const
    {
        std::vector<double> les1;
        std::vector<double> les2;
        std::set<double> check;
        if(this->corners.size() != rhs.corners.size())
        {
           std::cout <<" this " << this->corners[0]<<" "<< this->corners[1]<<"  "<< this->corners[2] <<"  "<<this->corners[3]<< std::endl;
           std::cout <<" rhs " << rhs.corners[0]<<" "<< rhs.corners[1]<<"  "<< rhs.corners[2] <<"  " <<this->corners[3]<< std::endl;
            std::set<double> check(this->corners.begin(), this->corners.end());
            int count=0;
            for(auto ele : rhs.corners)
            {
                if(check.insert(ele).second)
                {

                }
                else
                {
                //    std::cout << ele << std::endl;
                   // check.insert(ele);
                    count=count+1;
                }
            }
            if(count==3)
            {
               std::cout << "count hit \n";
                return false;
            }
            else {

            }
        }
        if(this->corners.size() ==4 && rhs.corners.size()==4)
        {
            std::cout <<"from 4 vec callwdq fcevdsnclksnv " << std::endl;
            return std::make_tuple(corners[0],corners[1],corners[2],corners[3]) < std::make_tuple(rhs.corners[0],rhs.corners[1],rhs.corners[2],corners[3]);
        }
        return std::make_tuple(corners[0],corners[1],corners[2]) < std::make_tuple(rhs.corners[0],rhs.corners[1],rhs.corners[2]);
    }
    friend std::ostream & operator <<(std::ostream &out , const point &rhs)
    {
        for(auto ele : rhs.corners)
        {
            out << ele <<" ";
        }
        out <<"\n";
        return out;
    }
private:
   std::vector<double> corners;
};




/*typedef Face3D<Point_3* , point> tri;
int main() {
    std::map<tri, std::string> pm;
    Point_3* p0 = new Point_3 (1.0,2.0,3.0);
    p0->setID(0);
    Point_3* p1 = new Point_3 (4.0,5.0,6.0);
    p1->setID(1);
    Point_3* p2 = new Point_3 (7.0,8.0,9.0);
    p2->setID(2);
    Point_3* p3 = new Point_3 (11.0,12.0,13.0);
    p3->setID(3);
    Point_3* p4 = new Point_3 (21.0,22.0,23.0);
    p4->setID(4);
    Point_3* p5 = new Point_3 (15.0,25.0,35.0);
    p5->setID(5);

    tri obj1(p0,p1,p2, nullptr);
    tri obj2(p0,p1,p3, nullptr);
    tri obj3(p0,p1,p4,p5, nullptr);
    pm.insert({obj1,"found"});
    pm.insert({obj2,"found"});
    pm.insert({obj3,"found"});
    std::cout <<"--------------------------------------" << std::endl;
    std::cout << "size of map os  " << pm.size() << std::endl;
    for(auto ele : pm)
    {
        std::cout << ele.first << std::endl;
    }
    tri obj4(p0,p4,p1, nullptr);
    std::cout <<"--------------------------------------" << std::endl;
    std::cout << pm[obj4] << std::endl;*/


//point* p = new point(4,5,6);
//void * ptr= nullptr;
//ptr=p;
//point* yy =  reinterpret_cast<point*>(ptr);
//std::cout <<*yy<< std::endl;
//
//typedef face<point*> fobj;
//fobj ele(1,2,p);
//}
int main() {
    const std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/ProjectSKA/cmake-build-debug/codes/shared0.vtk";
 //    const std::string filename="/home/aravindhkumar.kalimuthu/Codes/3D_DS/3D/tetalone.vtk";
   TMesh tmesh;
    try
    {
        tmesh = ReadVtk(filename);
    }
    catch (const std::string &msg)
    {
        std::cout << msg << std::endl;
    }
    tmesh->printContainerInfo();
    tmesh->kill();
    std::cout << "------------------------------------------" << std::endl;
    tmesh->printContainerInfo();
  /*std::vector<Element*> allpoly;
    tmesh->getalivePolygons(allpoly);
    Element* el = allpoly.at(0);

    TetElement::Tetrahedron* tetra = dynamic_cast<TetElement::Tetrahedron*>(el);
    Point_3* p1 = tetra->getCorner(0);
    Point_3* p2 = tetra->getCorner(1);
    Point_3* p3 = tetra->getCorner(2);
    Point_3* p4 = tetra->getCorner(3);
    size_t triid[3]={0,0,0};
    TriElement::Triangle* t1 = new TriElement::Triangle(p2,p3,p4,triid, nullptr);
    TriElement::Triangle* t2 = new TriElement::Triangle(p1,p4,p3,triid, nullptr);
    TriElement::Triangle* t3 = new TriElement::Triangle(p1,p2,p4,triid, nullptr);
    TriElement::Triangle* t4 = new TriElement::Triangle(p1,p3,p2,triid, nullptr);
    std::vector<TriElement::Triangle*> triec={t1,t2,t3,t4};
    writeSTL("new",triec);*/
  //  std::cout << *tetra << std::endl;
//  tmesh->getBorderFaces();
 // tmesh->printContainerInfo();
 /*   TMesh mesh (new Mesh3D());
    Point_3 * p1 = mesh->createPoint_3(1,2,3);
    Point_3 * p2 = mesh->createPoint_3 (11,22,3);
    Point_3 * p3 = mesh->createPoint_3 (11,24,3);
    Point_3 * p4 = mesh->createPoint_3 (1,24,43);


    Point_3 * p5 = mesh->createPoint_3(1,2,3);
    Point_3 * p6 = mesh->createPoint_3 (11,22,3);
    Point_3 * p7 = mesh->createPoint_3 (11,24,3);
    Point_3 * p8 = mesh->createPoint_3 (1,24,43);


   auto elet1  = mesh->createTetrahedron(p1,p2,p3,p4);
    auto elet2 =  mesh->createTetrahedron(p1,p2,p3,p4);
  auto ele1 = mesh->createTriangle(p1,p2,p3);
    auto ele2 = mesh->createTriangle(p1,p2,p3);

   mesh->printContainerInfo();
   if(*ele1 == *ele2)
   {
       std::cout << "yes equal " << std::endl;
   }
    if(*elet1 == *elet2)
    {
        std::cout << "yes equal " << std::endl;
    }
*/
//    tmesh->getBorderFaces();
}
// TMesh tmesh = ReadVtk("/home/aravindhkumar.kalimuthu/Codes/3D_DS/3D/tetalone.vtk");
// tmesh->printContainerInfo();