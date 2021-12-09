#include "../Mesh.h"


class PlaneClipping {
public:
    PlaneClipping() {
        std::cout << "Plane Clipping ALgorithm !! " << std::endl;
    }
    PlaneClipping(std::shared_ptr<Mesh> &mesh);
    void computePlane();
    void clipping(const Vector3 &point, const Vector3 &normal);
    int getPositiveID(const std::array<int,3> &array);
    int getNegativeID(const std::array<int,3> &array);
    Point* computeIntersection(double d1, double d2, Point* p1, Point* p2);
    void createTriangle( std::map<unsigned int, std::vector<Point*>> & idtotri);
private:
    std::shared_ptr<Mesh> pMesh = nullptr;
    std::shared_ptr<Mesh> pMeshClipped = nullptr;
};

PlaneClipping::PlaneClipping(std::shared_ptr<Mesh> &mesh) {
    pMesh = mesh;
   // std::shared_ptr<Mesh> pMeshtemp(new Mesh());
    pMeshClipped = std::make_shared<Mesh>();
    computePlane();
}

void PlaneClipping::computePlane() {
    Bbox_3 bobj = pMesh->getBBOX();
    std::cout << "Bbox is -----> " << bobj << std::endl;
    double arrX[2] = {bobj.xmin(), bobj.xmax()};
    double arrY[2] = {bobj.ymin(), bobj.ymax()};
    double arrZ[2] = {bobj.zmin(), bobj.zmax()};
//    arrX[0] = bobj.xmin();
//    arrX[1] = bobj.xmax();
//    arrY[0] = bobj.ymin();
//    arrY[1] = bobj.ymax();
//    arrZ[0] = bobj.zmin();
//    arrZ[1] = bobj.zmax();
  //  std::cout<<arrX[0]<<" "<<arrY[0]<<" "<<arrZ[0]<<" "<<arrX[1]<<" "<<arrY[1]<<" "<<arrZ[1]<<" "<<std::endl;
  std::vector<Vector3> planePoints;
    for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) {
            for(int k=0; k<2; k++) {
                Vector3 pobj(arrX[k],arrY[j],arrZ[i]);
                planePoints.push_back(pobj);
            //    std::cout << arrX[k] << " " << arrY[j] << " "<<arrZ[i] << std::endl;
            }
        }
    }
    double a = bobj.zmax();
    double b = bobj.zmin();
    for(auto ele : planePoints) {
        std::cout << ele << std::endl;
    }
    std::cout << std::endl<< std::endl;
    std::cout <<"range  " << (a) -  (b) << std::endl;
//
    Vector3 newVer_0(planePoints.at(4).x(),planePoints.at(4).y(),(planePoints.at(4).z())-58 );
    Vector3 newVer_1(planePoints.at(5).x(),planePoints.at(5).y(),(planePoints.at(5).z())-58);
    Vector3 newVer_2(planePoints.at(6).x(),planePoints.at(6).y(),(planePoints.at(6).z())-58);
    Vector3 newVer_3(planePoints.at(7).x(),planePoints.at(7).y(),(planePoints.at(7).z())-58);
    Vector3 edvec1 = newVer_1- newVer_0;
    Vector3 edvec2 = newVer_2- newVer_0;
//    std::cout << "plane point " << planePoints.at(4) << std::endl;
//    std::cout << "newVer_0 " << newVer_0 << std::endl;
//    Vector3 newVer_2(planePoints.at(2).x(),(planePoints.at(2).y())-80,planePoints.at(2).z() );
//    Vector3 newVer_3(planePoints.at(3).x(),(planePoints.at(3).y())-80,planePoints.at(3).z());
//    Vector3 newVer_6(planePoints.at(6).x(),(planePoints.at(6).y())-80,planePoints.at(6).z());
//    Vector3 newVer_7(planePoints.at(7).x(),(planePoints.at(7).y())-80,planePoints.at(7).z());
//
//    Vector3 edvec1 = newVer_7- newVer_6;
//    Vector3 edvec2 = newVer_2- newVer_6;

//    std::cout << "edvec1 " <<edvec1 << std::endl;
//    std::cout << "edvec2 " <<edvec2 << std::endl;
    Vector3 normal = cross_product(edvec1,edvec2);
    //std::cout << normal  <<  std::endl;
    normal.make_unit_vector();
  //  std::cout <<"afetr " <<normal  <<  std::endl;
    clipping(newVer_0,normal);

}

void PlaneClipping::clipping(const Vector3 &point, const Vector3 &normal) {

    std::vector<Point *> allpts;
    std::vector<Point *> eliminate;
    std::vector<Point *> keep;
    pMesh->getVerticesAll(allpts);
    std::map<unsigned int, double> vertexID_to_distance;
    for (auto ele: allpts) {
        Vector3 current_Vert = to_Vector3(ele);
        Vector3 pervec = current_Vert - point;
//        if(ele->ID == 71 || ele->ID == 70) {
//            std::cout << "point  is " << *ele << std::endl;
//        }
        double distance = dot(normal, pervec);
        if (distance >= 0) {
            eliminate.push_back(ele);
            vertexID_to_distance.insert({ele->ID, distance});
        } else {
            vertexID_to_distance.insert({ele->ID, distance});
            keep.push_back(ele);
        }
    }


    assert(vertexID_to_distance.size() == allpts.size());
    //std::cout <<  vertexID_to_distance.size() <<" " << allpts.size()<< std::endl<< std::endl;
    std::cout << "keep " << keep.size() << " " << "eliminate  " << eliminate.size() << std::endl;

//    std::vector<EdgeOrder> alledges;
//    std::vector<Triangle*> alltri;
//    pMesh->getTriangles(alltri);
//    getEdgesofTrianlges(alltri,alledges);
//
//    for(const EdgeOrder &ed : alledges) {
//        double d1 = vertexID_to_distance[ed.p0->ID];
//        double d2 = vertexID_to_distance[ed.p1->ID];
//        if((d1 * d2) >= 0) {
//            continue;
//        }
//        std::cout << "edge is     " << ed << std::endl;
//        break;
//    }

//   writePoints("keep_bunny.vtk",keep);
  //     writePoints("eliminate_bunny.vtk", eliminate);

    std::vector<std::vector<Triangle*>> triTypes;
    for(int y=0; y<4; y++) {
        std::vector<Triangle*> a;
        triTypes.push_back(a);
    }
  //  std::cout << "tri types " <<triTypes.size() << std::endl;
    std::vector<Triangle *> alltri;
    pMesh->getTriangles(alltri);
    std::map<unsigned int, std::array<int,3>> triID_to_sign;
    for (auto f: alltri) {
        Triangle *t(f);
        int positive = 0;
        int negative = 0;
        std::array<int,3> arr{0,0,0};
        for (int i = 0; i < 3; i++) {
            Point* srcp = t->getCorners(i);
//            if(t->ID == 25) {
//                std::cout <<"printing   " << std::endl;
//                std::cout <<*t<<std::endl;
//            }
            double dis = vertexID_to_distance[srcp->ID];
            if(dis >=0) {
                positive = positive+1;
                arr[i]=1;
            }else {
                negative = negative+1;
                arr[i]=-1;
            }
        }
        triID_to_sign.insert({f->ID,arr});
        if(positive==2 && negative==1) {
            triTypes.at(1).push_back(f);
        } else if(positive==1 && negative==2) {
            triTypes.at(2).push_back(f);
        } else if(negative == 3) {
            triTypes.at(3).push_back(f);
        }else {
            triTypes.at(0).push_back(f);
            //positive ==3;
        }
    }

//    for(int e=0; e<ar.size(); e++) {
//        std::cout <<"sign is      " <<ar[e]<< std::endl;
//    }

std::vector<Point*> outpoint;
    for(int w=0; w<triTypes.at(3).size(); w++) {
        Triangle *t = triTypes.at(3).at(w);
        std::vector<Point*> tri1={t->getCorners(0), t->getCorners(1), t->getCorners(2)};
        std::map<unsigned int, std::vector<Point*>> idtotri;
        idtotri.insert({0,tri1});
        createTriangle(idtotri);
    }
    std::vector<Point*> positivepoints;
    for(int w=0; w<triTypes.at(2).size(); w++) {
        Triangle *t = triTypes.at(2).at(w);
        auto array_ = triID_to_sign[t->ID];


     //   std::cout <<array_[0]<<" "<<array_[1]<<" "<<array_[2]<<std::endl;
       std::cout<< getPositiveID(array_) << std::endl;
        int posiID = getPositiveID(array_);
        positivepoints.push_back(t->getCorners(posiID));
        int edgeID_1 = indexOrder_1(posiID);
        int edgeID_2 = indexOrder_2(posiID);
        std::set<int> edids={edgeID_1,edgeID_2};
        Point* parr[3]={nullptr, nullptr, nullptr};
        while (!edids.empty()){
            int startid = *edids.begin();
        //    std::cout <<"startid "<<startid<<std::endl;
            Point* p1 = t->getCorners(indexOrder_1(startid));
            Point* p2 = t->getCorners(indexOrder_2(startid));
            double d1 = vertexID_to_distance[p1->ID];
            double d2 = vertexID_to_distance[p2->ID];
            Point* interp= computeIntersection(d1,d2,p1,p2);
            parr[startid] = interp;
            edids.erase(startid);
        }
     //   std::cout << *t << std::endl;
        for(int ii=0; ii<3; ii++) {
            if(parr[ii]== nullptr) continue;
         //   std::cout << *parr[ii] << std::endl;
        }
        std::vector<Point*> tri1={t->getCorners(indexOrder_1(posiID)), t->getCorners(indexOrder_2(posiID)), parr[indexOrder_2(posiID)]};
       std::vector<Point*> tri2={parr[indexOrder_2(posiID)],t->getCorners(indexOrder_2(posiID)), parr[indexOrder_1(posiID)]};
        std::map<unsigned int, std::vector<Point*>> idtotri;
        idtotri.insert({0,tri1});
        idtotri.insert({1,tri2});
//        if(w==3) {
//            std::cout << *t << std::endl;

//            createTriangle(idtotri);
//            break;
//        }
            for(int f=0; f<3; f++) {
            outpoint.push_back(t->getCorners(f));
        }
            createTriangle(idtotri);
 //break;
    }
//    writePoints("triangle",outpoint);
std::vector<Point*> negativepoints;
    for(int w=0; w<triTypes.at(1).size(); w++) {
        Triangle* t = triTypes.at(1).at(w);
     //   std::cout << *t << std::endl;
//        for(int f=0; f<3; f++) {
//            outpoint.push_back(t->getCorners(f));
//        }
        auto array_ = triID_to_sign[t->ID];
      //  std::cout <<array_[0] << " " <<array_[1] << " " <<array_[2]<< std::endl;
        int negativeid = getNegativeID(array_);
        negativepoints.push_back(t->getCorners(negativeid));
//        std::cout << "negativeid " <<negativeid<<std::endl;
   //  std::cout << t->ID << std::endl;
        Point* p1 = t->getCorners(indexOrder_1(negativeid));
        Point* p2 = t->getCorners(indexOrder_2(negativeid));
        double d2, d1,d3;
        d1 = vertexID_to_distance[t->getCorners(negativeid)->ID];
        d2 = vertexID_to_distance[p1->ID];
        d3 = vertexID_to_distance[p2->ID];

        Point* np1= computeIntersection(d1,d2,t->getCorners(negativeid),p1);
        Point* np2= computeIntersection(d1,d3,t->getCorners(negativeid),p2);
//        std::cout << *np1 << std::endl;
//        std::cout << *np2 << std::endl;
        Point* arraypoint[3];
        arraypoint[negativeid]=t->getCorners(negativeid);
        arraypoint[indexOrder_1(negativeid)]=np1;
        arraypoint[indexOrder_2(negativeid)]=np2;
        std::map<unsigned int, std::vector<Point*>> ptstotri;
        std::vector<Point*> onetri;

        onetri.push_back(arraypoint[0]);
        onetri.push_back(arraypoint[1]);
        onetri.push_back(arraypoint[2]);
        ptstotri.insert({0,onetri});
        createTriangle(ptstotri);
     /*   outpoint.push_back(p1);
        outpoint.push_back(p2);
        outpoint.push_back(t->getCorners(negativeid));
        std::cout << *p1 <<std::endl;
        std::cout << *p2 <<std::endl;

        std::cout <<"idsss  " <<p2->ID << " " << p1->ID << std::endl;
        std::map<unsigned int, Point*> id_to_point;
        Point* arraypoint[3];
        id_to_point.insert({negativeid,t->getCorners(negativeid)});
        arraypoint[negativeid]=t->getCorners(negativeid);
     //   float r = d2 / (d2 - d1);
//        Point* new1 = new Point(t->getCorners(negativeid)->x()*r,t->getCorners(negativeid)->y()*r,t->getCorners(negativeid)->z()*r, nullptr);
//   //     Point* new2 = new Point(p2->x()*(1.0f - r),p2->y()*(1.0f - r),p2->z()*(1.0f - r), nullptr);
//        Point* new2 = new Point(p1->x()*(1.0f - r),p1->y()*(1.0f - r),p1->z()*(1.0f - r), nullptr);
//        Point* finalp = *new1+*new2;
//        std::cout <<"final " << *finalp << std::endl;
        Point* np1= computeIntersection(d1,d2,t->getCorners(negativeid),p2);
        Point* np2= computeIntersection(d1,d3,t->getCorners(negativeid),p1);
        id_to_point.insert({indexOrder_2(negativeid),np1});
        id_to_point.insert({indexOrder_1(negativeid),np2});
        arraypoint[indexOrder_2(negativeid)]=np1;
        arraypoint[indexOrder_1(negativeid)]=np2;
        std::map<unsigned int, std::vector<Point*>> ptstotri;
        std::vector<Point*> onetri;

        onetri.push_back(arraypoint[0]);
        onetri.push_back(arraypoint[1]);
        onetri.push_back(arraypoint[2]);
        ptstotri.insert({0,onetri});
       // tripoints.push_back(onetri);
        for(auto ele : id_to_point){
            std::cout << ele.first << " " << " " << *ele.second << " "<<" "<< *arraypoint[ele.first]<< std::endl;
        }
    //    computeIntersection(d1,d3,t->getCorners(negativeid),p1);
      createTriangle(ptstotri);*/



    }
  //  writePoints("negative",negativepoints);
   // writePoints("positive",positivepoints);
  //  writePoints("tri",outpoint);
  //  float r = d2 / (d2 - d1);
    /*Add the edge intersection point to the vertices vector and track its index.*/
 //   trackedIdx[ei + numVertices()] = verts.size();
//    verts.push_back((vertex(edge.p) * r) + (vertex(edge.q) * (1.0f - r)));


  //  std::cout <<"getid is  " << getID(ar) << std::endl;
//    int sz_=0;
//    for(auto ele :triTypes) {
//        sz_=sz_+ele.size();
//    }
//    std::cout << "sz_ size is   " << sz_    << std::endl;


  //  pMeshClipped->writeMeshSTL("clipped");
}

int PlaneClipping::getNegativeID(const std::array<int,3> &array) {
    for(int i=0; i<3; i++) {
        if(array[i] < 0) return i;
    }
}

int PlaneClipping::getPositiveID(const std::array<int,3> &array) {
    for(int i=0; i<3; i++) {
        if(array[i] > 0) return i;
    }
}
Point* PlaneClipping::computeIntersection(double d1, double d2, Point* p1, Point* p2) {

    float r = d2 / (d2 - d1);
    Point* new1 = new Point(p1->x()*r,p1->y()*r,p1->z()*r, nullptr);
    Point* new2 = new Point(p2->x()*(1.0f - r),p2->y()*(1.0f - r),p2->z()*(1.0f - r), nullptr);
    Point* finalp = *new1+*new2;
 //   std::cout <<"from final " << *finalp << std::endl;
    return finalp;
}

void PlaneClipping::createTriangle( std::map<unsigned int, std::vector<Point*>> & idtotri) {

    for(int i=0; i<idtotri.size(); i++) {
       std::vector<Point*> fv = idtotri[i];
       Point* p1 = pMeshClipped->createVertex(fv[0]->x(),fv[0]->y(),fv[0]->z());
       Point* p2 = pMeshClipped->createVertex(fv[1]->x(),fv[1]->y(),fv[1]->z());
        Point* p3 = pMeshClipped->createVertex(fv[2]->x(),fv[2]->y(),fv[2]->z());
        pMeshClipped->createTriangle(p1,p2,p3, nullptr);
    }


}


/*std::vector<Triangle*> triclipped;
pMeshClipped->getTriangles(triclipped);
std::vector<Triangle*> neigh;
std::vector<EdgeOrder> edes;
getEdgesofTrianlges(triclipped,edes);
std::multimap<Point*, EdgeOrder,ComparePoint> point_to_edge;
std::set<Point*,ComparePoint> alledpts;
for(auto ele : edes) {
point_to_edge.insert(std::make_pair(ele.p0,ele));
point_to_edge.insert(std::make_pair(ele.p1,ele));
alledpts.insert(ele.p0);
alledpts.insert(ele.p1);
}
typedef std::multimap<Point*, EdgeOrder,ComparePoint>::iterator IT;
for(auto ele_ : alledpts ) {
std::pair<IT,IT> edges = point_to_edge.equal_range(ele_);
int distance = std::distance(edges.first,edges.second);
if (distance ==3 ) {
std::cout<< "and the pts is    " << *ele_ << std::endl;
for(IT iter = edges.first; iter != edges.second; iter++) {
std::cout <<"edge is  " << iter->second << " " << iter->second.p0->ID<<" "<< iter->second.p1->ID<< std::endl;
}
break;
}
}
std::vector<Point*> allptsclipped;
pMeshClipped->getVertices(allptsclipped);
std::vector<Point*> edvec;
for(auto ele: allptsclipped) {
if(ele->ID==1123 || ele->ID==745) {
std::cout << "sele ele is  " << *ele << std::endl;
edvec.push_back(ele);
}
else{
continue;
}
}
std::vector<Triangle*> neight;
pMeshClipped->getAdjustenNeigh_1(EdgeOrder(edvec.at(0),edvec.at(1)),neight );
std::cout <<"neight is " << neight.size() << std::endl;*/
