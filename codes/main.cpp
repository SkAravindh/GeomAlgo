#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"
#include "PlaneClipping.cpp"
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


   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunny_S_Up_R.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/Nozzle_remesh_5mm.stl";
 //   std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/ProjectSKA/cmake-build-debug/codes/clipped.stl";
    std::shared_ptr<Mesh> pMesh = ReadSTL(filename);
   // pMesh->printContainersInfo();
  //  std::cout << pMesh->getBBOX() << std::endl;
    PlaneClipping obj(pMesh);
  /*std::vector<Triangle*> alltri;
  pMesh->getTriangles(alltri);
  std::vector<EdgeOrder> edges;
    getedgesByOrder(alltri,-1,edges);
    std::set<Point*> bp;
    for(auto ele: edges) {
        bp.insert(ele.p0);
        bp.insert(ele.p1);
    }
    std::vector<Point*> bpvec;
    bpvec.assign(bp.begin(),bp.end());
    writePoints("bp_points",bpvec);*/
   // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/Cube_Remesh.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/RotatedPlate_mm_25mm.stl";


    // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/morefiles_STL/Torus.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/283.stl";
  //  std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/failing_model.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/Fluid_volume/Pipe_actual.stl";
    //std::string filename= "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/Injektor_actual_geometry_R.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/Pipe_Remesh.stl";
   // std::string filename="/home/aravindhkumar.kalimuthu/Downloads/testing_stls/Fluid_volume/Griffschale_actual_geometry.stl";

    //  std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/Case3.stl";
   // std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/case3simplified.stl";
   // std::string filename ="/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/input_Geometry.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/Bracket.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/unorderedmap/cmake-build-debug/masterMesh_HoleCreated.stl";
  //  std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/Volute_2mm.stl";

    //   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/Cube_3d_printing_sample.stl";
    //std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/testmodels/BMW_vibration.stl";

        //injector_cvt.stl
        //injector_from_nas.stl
    //outFile.stl
    //pipe_offset.stl
   //  std::shared_ptr<Mesh> pMesh_nas = ReadSTL("/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/pipe_offset.stl");
   // std::shared_ptr<Mesh> pMesh = ReadSTL("/home/aravindhkumar.kalimuthu/Downloads/testing_stls/fluid_domain/remesh/injector_cvt.stl");
   // std::shared_ptr<Mesh> pMesh = ReadSTL("/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/outFile.stl");
    //compare(pMesh_nas,pMesh);
    //d=aa;


  /*  std::vector<Point*> allpoints;
    pMesh->getVerticesAll(allpoints);
    std::vector<Triangle*> ringtri;
    pMesh->getRingNeigbyOrder(allpoints[0],1,ringtri);
    Vector3 nnv(0,0,0);
    std::cout<<"allpoints[0] "<<*allpoints[0]<<std::endl;
    for(auto ele : ringtri) {
        Vector3 a = ele->getNormalVector().make_unit_vector();
        nnv = nnv+ a;
        std::cout<<"a " <<a<<std::endl;
    }
    std::cout<<"nnv " <<nnv/ringtri.size()<<std::endl;
   */


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






 








}
