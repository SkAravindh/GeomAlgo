#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"
//#include "verification/FluidDomain.cpp"
//#include "verification/adaptdesire_version2_Send.cpp"
/*#include "verification/adaptdesire_version2_1_Send.cpp"

std::string getFileName(const std::string& s) {

    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    if (i != std::string::npos) {
        return(s.substr(i+1, s.length() - i));
    }

    return("");
}*/


int main() {

  // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunny_S_Up_R.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/morefiles_STL/Torus.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/283.stl";
  //  std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/failing_model.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/Fluid_volume/Pipe_actual.stl";
    std::string filename= "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/Injektor_actual_geometry_R.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/Pipe_Remesh.stl";
   // std::string filename="/home/aravindhkumar.kalimuthu/Downloads/testing_stls/Fluid_volume/Griffschale_actual_geometry.stl";






    //  std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/Case3.stl";
   // std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/case3simplified.stl";
   // std::string filename ="/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/input_Geometry.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/Bracket.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/unorderedmap/cmake-build-debug/masterMesh_HoleCreated.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/Volute_2mm.stl";

    //   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/Cube_3d_printing_sample.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/testmodels/BMW_vibration.stl";


    // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/completecar.stl";
  //  std::string filename="/home/aravindhkumar.kalimuthu/Downloads/testing_stls/patch.stl";

    //std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/single_cube_holes_stl.stl";
 //   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunnywithseveralholes.stl";


  //  std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/bpillar.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/BMW.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/build/codes/reout1.stl";

//    std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/PipeTShape_1.stl";



     //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/reout.stl";

     std::shared_ptr<Mesh> pMesh = ReadSTL(filename);
 

    //Fluid_Domain obj(pMesh);
    //obj.identify_holes();
    //obj.fun();
   // obj.establish_connectivity();
   // pMesh->writeMeshSTL("insidetri");
//    if(pMesh->is_Solid(nullptr, nullptr)) {
//        std::cout<<"it is solid " <<std::endl;
//    }

    /*std::random_device rd;
    std::random_device rd_1;
    std::random_device rd_2;
    std::default_random_engine generator(rd());
    //std::default_random_engine generator_1(rd_1());
    //std::default_random_engine generator_2(rd_2());
    std::uniform_real_distribution<double> distribution(-2, 2);
   // std::uniform_real_distribution<double> distribution_1(-2, 2);
    //std::uniform_real_distribution<double> distribution_2(-3, 3);
    for (int i=0; i<100; ++i) {
       // std::cout << distribution(generator) << "\n";
       // std::cout << "y " << " " << distribution(generator) << "\n";
       // std::cout << "z " << " " << distribution(generator) << "\n";
        double x = distribution(generator);
        double y = distribution(generator);
        double z = distribution(generator);
        Point* p = new Point(x,y,z);
        allp.push_back(p);
    }
  //  pMesh->getVertices(allp);
   // writePoints("allpoints.vtk", allp);
    ConvexHull a(allp);
    a.initilize0utpoints();
    a.createInitialSimplex();
*/







 /*   DesiredEdge2_1Send obj(pMesh);
       obj.doRefine(0);

         pMesh->writeMeshSTL("reout_case3.stl");
         pMesh->printContainersInfo();*/

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
