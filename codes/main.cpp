#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"

//#include "Algorithms/HoleDetection.h"


//#include "verification/adaptdesire_version2.cpp"
//#include "verification/adaptdesire_version2_Send.cpp"
//#include "verification/adaptdesire_version2_1_Send.cpp"







int main() {
//   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunny_S_Up_R.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/Bracket.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/unorderedmap/cmake-build-debug/masterMesh_HoleCreated.stl";
    std::string filename = "/home/aravindhkumar.kalimuthu/Codes/TetMesher/TetMesher_Ubuntu18.04_x86_64/Volute_2mm.stl";

    //   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/Cube_3d_printing_sample.stl";


      // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/completecar.stl";
  //  std::string filename="/home/aravindhkumar.kalimuthu/Downloads/testing_stls/patch.stl";

  //  std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/single_cube_holes_stl.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunnywithseveralholes.stl";


    //std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/bpillar.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/BMW.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes/reout1.stl";

//    std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/PipeTShape_1.stl";



     //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/reout.stl";
  //  std::shared_ptr<Mesh> pMesh = ReadSTL(filename);
        std::shared_ptr<Mesh> mesh(new Mesh);
    /* Point* p0 = mesh->createVertex(2,4,2);
     Point* p1 = mesh->createVertex(14,8,3);
     Point* p2 = mesh->createVertex(7,14,3);
     Point *p= new Point(10,10,2);*/
    Point* p0 = mesh->createVertex(4,4,0);
    Point* p1 = mesh->createVertex(16,8,0);
    Point* p2 = mesh->createVertex(10,14,0);
    Point *p= new Point(16,12,0);
     Triangle* tri = new Triangle(p0,p1,p2,&*mesh);
     Vector3* a = new Vector3();
    std::cout << getMinimumDistance(tri,p) << std::endl;
   /*  bool result = brayCentric(tri,p,a);
     std::cout<<"ortho " <<getOrthogonalProjectionVector(tri,p) << std::endl;
     if(result) {
         std::cout<< "true " <<" "<<" "<< *a << std::endl;

     }*/
   Vector3 v1(2,4,2);
    Vector3 v2(14,8,3);
    Vector3 vp(6,3,3);
   // segmentPointDistance(v1,v2,vp);


   //std::cout<< "dis " <<   getMinimumDistance(tri,p) << std::endl;
    /* std::set<int> triID;
     triID.insert(37985);
    triID.insert(37996);
    triID.insert(38003);
    triID.insert(38034);
    triID.insert(38064);
    triID.insert(179239);
    triID.insert(179240);
    triID.insert(179246);
    triID.insert(179247);
    triID.insert(179251);
    s
    pMesh->printContainersInfo();

    std::vector<Triangle*> alltri;
    pMesh->getTriangles(alltri);
    std::vector<Triangle*> tri_to_del;
    for(int i=0; i<alltri.size(); i++ ) {
        if(triID.find(i) != triID.end() ) {
           tri_to_del.push_back(alltri[i]);
        }
    }
    writeSTL("bad.stl",tri_to_del); */
    /*DesiredEdge2_1Send obj(pMesh);
    obj.doRefine(0);
    obj.doRefine(2000);
      pMesh->writeMeshSTL("reout1.stl");*/
 //   obj.doRefine(4000);
//    std::vector<Triangle*> alltri;
//    std::vector<EdgeOrder> alledges;
//    pMesh->getTriangles(alltri);
//    std::cout<<"alltri " <<alltri.size() << std::endl;
//    getedgesByOrder_1(alltri,1,alledges);

  /*  std::vector<Triangle*> alltri;
    std::vector<Triangle*> ringtri;
    pMesh->getTriangles(alltri);
    pMesh->getRingNeigbyOrder(alltri[0]->getCorners(0),1,ringtri);
    Point* inputpoint = alltri[0]->getCorners(0);

    std::vector<EdgeOrder> alledges;
    std::cout<<"Inputpoint "<<*inputpoint<<" "<<"ringtri size " <<ringtri.size() << std::endl;
   // getEdgesofTrianlges(ringtri,alledges);
   //getedgesByOrder(ringtri,-2,alledges);
   for(auto ele : ringtri) {
   int id=  ele->getVertexID(inputpoint);
     std::cout<<*(ele->getCorners(indexOrder_1(id)))<<" " << " "<<*(ele->getCorners(indexOrder_2(id))) <<std::endl;
   }
    
    for(auto ele : alledges) {
      std::cout << ele << std::endl;
    }*/
    //std::cout << *alltri[0] << std::endl;


  //  obj.doRefine(6000);
    //obj.doRefine(3000);
  // obj.doRefine(500);
   // std::cout << "another -------------- " << std::endl;
 //   DesiredEdge2Send obj1(pMesh);
  //  obj1.doRefine(0);
 //   obj.findTriToFixWindError();
 //   obj.doRefine(3000);
   // obj.findTriToFixWindError();
  // pMesh->writeMeshSTL("bunny.stl");
   // pMesh->printContainersInfo();

    //edgedesiredversion_2
//    DesiredEdge_2 obj(pMesh);
//    obj.do_refine();



//edgedesired
//    DesiredEdge obj(pMesh);
//    obj.do_refine();
//    pMesh->writeMeshSTL("finall1.stl");

//        std::vector<Triangle*> alltv;
//        pMesh->getTriangles(alltv);
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

  //  std::set<EdgeOrder> edset;
//    std::vector<Triangle*> allt;
//    pMesh->getTriangles(allt);
//    for(auto ele : allt) {
//        for(int i=0; i<3; i++) {
//            EdgeOrder* ed = ele->getEdge(i);
//            if(ed->status==EdgeStatus::Border) {
//                edset.insert(*ed);
//            }
//        }
//    }
//    std::cout << "edset " << edset.size() << std::endl;



 


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
