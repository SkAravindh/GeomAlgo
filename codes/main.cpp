#include <iostream>
#include "InOut.h"
#include "Mesh.h"
#include "Point.h"
#include "Collision.h"
#include "Bbox_3.h"
#include "EdgeOrder.h"
#include "Geom_Support.h"
#include "Writedata.h"
#include "LoopEdges.cpp"

//#include "verification/adaptdesire_version2_Send.cpp"
#include "verification/adaptdesire_version2_1_Send.cpp"
#include "verification/fixnonmanifold.cpp"

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
}

int main() {
  // std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/bunny_S_Up_R.stl";
 //   std::string filename = "/home/aravindhkumar.kalimuthu/Downloads/testing_stls/morefiles_STL/Torus.stl";
   // std::string filename = "/home/aravindhkumar.kalimuthu/Desktop/practice/Codes/projectSKA/cmake-build-debug/codes/car_box_changed.stl";
    std::string filename ="/home/aravindhkumar.kalimuthu/Downloads/case.stl";



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

   /* std::vector<Triangle*> allttri;
    std::cout<<pMesh->getCentroid()<<std::endl;
    std::cout<<pMesh->getCentroid(CentroidType::area_based)<<std::endl;
    std::cout<<pMesh->getCentroid(CentroidType::volume_based)<<std::endl;

    pMesh->translateMesh(tr);*/
    Point* tr = new Point(0.5,0.9,0.9);
    pMesh->translateMesh(tr);
  //  pMesh->writeMeshSTL("case_scaled_z.stl");
    //std::cout<<pMesh->getCentroid(CentroidType::vertex_based)<<std::endl;
  //  std::cout<<pMesh->getCentroid(CentroidType::area_based)<<std::endl;
  /* std::vector<Point*> allpoints;
    pMesh->getVertices(allpoints);
    std::cout<<"allpoints "<<allpoints.size()<<std::endl;
    std::map<Point*,Point*,ComparePoint> old_to_new;*/
   /*for(auto ele:allpoints) {
        double x = ele->x()/1000.0;
        double y = ele->y()/1000.0;
        double z = ele->z()/1000.0;
        Point* dummy= new Point(x,y,z);
        old_to_new.insert(std::make_pair(ele,dummy));
    }*/

   /*  for(auto ele:allpoints) {
         double x_ = ele->x();
         double y_ = ele->y();
         double z_ = ele->z();
         Vector3 dummyvec(x_,y_,z_);
         Vector3 transl( 0,0,0 );
         dummyvec+transl;
         dummyvec*1.05;
         Point* dummy= new Point(dummyvec.x(),dummyvec.y(),dummyvec.z());
         old_to_new.insert(std::make_pair(ele,dummy));
     }
    std::vector<Triangle*> alltri;
    pMesh->getTriangles(alltri);
    std::cout<<"alltri "<<alltri.size()<<std::endl;
    for(auto t : alltri) {
        for(int i=0; i<3; i++) {
           Point* replace =  t->getCorners(i);
           Point* newpoint = old_to_new[replace];
           t->setNewVertex(newpoint,i);
        }
    }

    std::cout<<"file writting has started " <<std::endl;
    pMesh->writeMeshSTL("case_0_1.05.stl");*/
    //Fixnonmani obj(pMesh);
    //obj.fixTriangle();
    //pMesh->writeMeshSTL("nonmani_tri.stl");

   /* std::vector<Point*> allpoint;
    pMesh->getVertices(allpoint);
    int count = 0;
    int pc=0;
    std::vector<Point*> sing;
    for(auto ele : allpoint){
        std::vector<Triangle*> ring;
        std::cout<<++pc<<std::endl;
        if(!ele->isAlive) continue;
       bool re = pMesh->isNon_Manifold_Vertex(ele, &ring);
        if(re){
            ++count;
            sing.push_back(ele);
        //    writeSTL(std::to_string(count)+".stl",ring);
        }
    }
    std::cout<<"non manifold vertices "<<"total point " <<allpoint.size()<<" " <<"non_mani count "<<count<< std::endl;
    writePoints("singulat.vtk",sing);*/
//    std::vector<Triangle*> ring1;
//    pMesh->getRingNeigbyOrder(nonmniverti[0],1,ring1);
//    writeSTL("ringadd.stl", ring1);
//    writePoints("points.vtk",nonmniverti);
/*std::cout<< *alltri[10987] << std::endl;
    std::vector<Triangle*> ring;
    pMesh->getRingNeigbyOrder(alltri[10987]->getCorners(0),3,ring);
if(*alltri[10987]->getCorners(0) == *alltri[10987]->getCorners(0)) {
    std::cout<<"true " << std::endl;
}
std::vector<EdgeOrder> edges;
    getedgesByOrder(ring,-1,edges);
    std::vector<Point*> bpoints;
    getBorderPoints(edges,bpoints);
    InitilizeLoop lo;
    lo.createLoop(edges);*/
    //writePoints("bpoints.vtk",bpoints);
 //   writeSTL("ring2.stl", ring);
  //  pMesh->writeMeshSTL("ring.stl");
   /* Point* p1 = new  Point(-39.75, 19.62, -54.0657);
    Point* p2 = new  Point(-40, 19.62, -53.25);
    Point* p3 = new  Point(-39.5, 19.62, -53.25);
    Point* p4 = new  Point(39.75, 19.62, -54.0657);
    Point* p5 = new  Point(39.5, 19.62, -53.25);
    Point* p6 = new  Point(40, 19.62, -53.25);
    Point* p7 = new  Point(6.10352e-05, 19.62, -53.9792);
    Point* p8 = new  Point(-0.25, 19.62, -53.25);
    Point* p9 = new  Point(0.25, 19.62, -53.25);
    Point* p10 = new  Point(-39.9607, 18.87, -53.125);
    Point* p11 = new  Point(-0, 18.8961, -53.2122);
    Point* p12 = new  Point(39.7499, 18.7931, -53.2068);
    Triangle* tri1 = mesh->createTriangle(p1,p2,p3, nullptr);
    Triangle* tri2 = mesh->createTriangle(p4,p5,p6, nullptr);
    Triangle* tri3 = mesh->createTriangle(p7,p8,p9, nullptr);
    Triangle* tri4 = mesh->createTriangle(p10,p2,p3, nullptr);
    Triangle* tri5 = mesh->createTriangle(p11,p9,p8, nullptr);
    Triangle* tri6 = mesh->createTriangle(p12,p6,p5, nullptr);
    std::vector<Triangle*> alltri = {tri1,tri2,tri3,tri4,tri5,tri6};
    writeSTL("fsbad_neigh.stl", alltri);*/

// Five neigh
  /*  Point* p1 = new  Point(-39.5, 19.62, -53.25);
    Point* p2 = new  Point(-40, 19.62, -52.907);
    Point* p3 = new  Point(-39.5, 19.62, -52.907);

    Point* p4 = new  Point(-39.75, 19.62, -54.0657);
    Point* p5 = new  Point(-40, 19.62, -53.25);
    Point* p6 = new  Point(-39.5, 19.62, -53.25);

    Point* p7 = new  Point(-39.9607, 18.87, -53.125);
    Point* p8 = new  Point(-40, 19.62, -53.25);
    Point* p9 = new  Point(-39.5, 19.62, -53.25);

    Point* p10 = new  Point(-39.7498, 18.8932, -53.2119);
    Point* p11 = new  Point(-39.5, 19.62, -53.25);
    Point* p12 = new  Point(-40, 19.62, -53.25);

    Point* p13 = new  Point(-40, 19.62, -53.25);
    Point* p14 = new  Point(-39.9802, 19.245, -52.8875);
    Point* p15 = new  Point(-40, 19.62, -52.907);
    Triangle* tri1 = mesh->createTriangle(p1,p2,p3, nullptr);
    Triangle* tri2 = mesh->createTriangle(p4,p5,p6, nullptr);
    Triangle* tri3 = mesh->createTriangle(p7,p8,p9, nullptr);
    Triangle* tri4 = mesh->createTriangle(p10,p11,p12, nullptr);
    Triangle* tri5 = mesh->createTriangle(p13,p14,p15, nullptr);
    Triangle* tri6 = mesh->createTriangle(p1,p5,p15, nullptr);
    std::cout<<tri6->getNormalVector()<<std::endl;*/
   // std::vector<Triangle*> alltri = {tri1,tri2,tri3,tri4,tri5};
 //   writeSTL("first_5neigh.stl", alltri);
   // writePoints("points.vtk",ps);
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
