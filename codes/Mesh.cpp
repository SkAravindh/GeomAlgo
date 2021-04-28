#include "Mesh.h"
#include "LoopEdges.cpp"

Mesh::Mesh() {

}

Mesh::Mesh(std::string Filename, size_t Poolsize) : ModelName(Filename), Poolsize(Poolsize) {

}

Point* Mesh::createVertex(const double x, const double y, const double z) {
    Point P(x,y,z);
    Point * Pptr = createVertex(P);
    return Pptr;
}

Point* Mesh::createVertex(const Point &P) {

    std::map<Point, Point*>::iterator it=allvertices.find(P);
    if(it != this->allvertices.end()){
        return it->second;
    }
    else{
        Point * pobj = new Point(P.x(),P.y(),P.z());
        this->allvertices.insert(std::make_pair(*pobj,pobj));
        vAllvertices.push_back(pobj);
        ++Number_OF_Vertices;
        pobj->isAlive = true;
        return pobj;
    }
}

Triangle* Mesh::createTriangle( Point *P0, Point *P1, Point *P2, Triangle* parent) {

    Triangle *newT = new Triangle(P0, P1, P2,this);
    if(parent == nullptr) {
        int size_ = allTriangles.size();
        newT->ID = size_;
        allTriangles.push_back(newT);
    }
    else {
        parent->addChild(newT);
    }
    return newT;
}

void Mesh::storeTriangleInfo(Triangle *T) {

    Point* p0 = T->getCorners(0);
    Point* p1 = T->getCorners(1);
    Point* p2 = T->getCorners(2);

    mmpointTotriangles.insert(std::make_pair(p0,T));
    mmpointTotriangles.insert(std::make_pair(p1,T));
    mmpointTotriangles.insert(std::make_pair(p2,T));

    mmedgeTotriangles.insert(std::make_pair(T->getEO(0),T));
    mmedgeTotriangles.insert(std::make_pair(T->getEO(1),T));
    mmedgeTotriangles.insert(std::make_pair(T->getEO(2),T));

    T->isAlive = true;
}

void Mesh::getAdjustenNeigh(const EdgeOrder& ed, std::vector<Triangle*> &tv) {

    std::vector<Triangle*> temp;
    std::pair<MMET, MMET> triangle_set = mmedgeTotriangles.equal_range(ed);
    if (std::distance(triangle_set.first, triangle_set.second) != 2) {
        std::cout << std::distance(triangle_set.first, triangle_set.second) << std::endl;
        std::cout << "Returning empty vector from ---> getAdjustenNeigh(Mesh.cpp)  " << std::endl;
        return;
    }
    for (MMET triangle_set_IT = triangle_set.first; triangle_set_IT != triangle_set.second; triangle_set_IT++) {
        temp.push_back(triangle_set_IT->second);
    }
    tv.assign(temp.begin(),temp.end());

}

void Mesh::getAdjustenNeigh_1(const EdgeOrder& ed,std::vector<Triangle*> &tv) {

    std::vector<Triangle*> temp;
    std::pair<MMET, MMET> triangle_set = mmedgeTotriangles.equal_range(ed);
    int dis = std::distance(triangle_set.first, triangle_set.second);
  //  if ( dis == 2 || dis == 1) {
        for (MMET triangle_set_IT = triangle_set.first; triangle_set_IT != triangle_set.second; triangle_set_IT++) {
            temp.push_back(triangle_set_IT->second);
        }
   // }
//    else {
//        std::cout << std::distance(triangle_set.first, triangle_set.second) << std::endl;
//        std::cout << "Returning empty vector from ---> getAdjustenNeigh_1(Mesh.cpp) and size is either 1 or 2  " << std::endl;
//        return;
//    }

    tv.assign(temp.begin(),temp.end());
}

void Mesh::establishNeighcompleteMesh() {

    TV_it it;
    for(it = allTriangles.begin(); it != allTriangles.end(); it++) {
        establishNeighofTriangle(*it);
    }
}

void Mesh::establishNeighofTriangle( Triangle* t) {

    Triangle* currentT(t);
    for(int i=0; i<3; i++) {
        EdgeOrder ed = currentT->getEO(i);
        std::vector<Triangle*> adjT;
        getAdjustenNeigh(ed,adjT);
        for(auto ele : adjT) {
            if(*ele == *currentT) {
                continue;
            } else {
                currentT->setNeigh(i,ele);
            }
        }
    }
}

void Mesh::reEstablishConnectivity() {
    //Call only when you completely update alltriangle container.
    mmpointTotriangles.clear();
    mmedgeTotriangles.clear();
    TV_it it;
    for(it=allTriangles.begin(); it != allTriangles.end(); it++) {
        storeTriangleInfo(*it);
    }
}

void Mesh::getTriangles(std::vector<Triangle*> &TV) {

    TV_it it;
    for(it=allTriangles.begin(); it!=allTriangles.end(); it++) {
        Triangle *tri(*it);
        if (tri->isAlive) {
            TV.push_back(tri);
        }
    }
}

void Mesh::getVertices(std::vector<Point*> &vp) {

    std::set<Point*> allpoints;
   // std::vector<Point*> vecpoint;
    TV_it it;
    for(it = allTriangles.begin(); it != allTriangles.end(); it++) {
        if(!(*it)->isAlive) continue;
        for(int i=0; i<3; i++) {
            Point* p = (*it)->getCorners(i);
            allpoints.insert(p);
       //     vecpoint.push_back(p);
        }
    }
 //   std::cout<< "vecpoint " <<vecpoint.size() << std::endl;
    vp.assign(allpoints.begin(),allpoints.end());
}

void Mesh::fillTriangleContainers(std::vector<Triangle*> &tv,VecContainerName VN ) {


    switch (VN) {
        case alltri:
            for(auto ele : tv) {
                int size_ = allTriangles.size();
                ele->ID=size_;
                allTriangles.push_back(ele);
            }
            break;
        case external:
            for(auto ele : tv) {
                externalUse.push_back(ele);
            }
            break;
        default:
            break;
    }
}

void Mesh::getTriangleContainers(std::vector<Triangle*> &tv,VecContainerName VN ) {

    switch (VN) {
        case alltri:
            for(auto ele : allTriangles){
                tv.push_back(ele);
            }
            break;
        case external:
            for(auto ele : externalUse){
                tv.push_back(ele);
            }
            break;
        default:
            break;
    }
}

void Mesh::clearTV(VecContainerName VN) {

    switch (VN) {
        case alltri:
            allTriangles.clear();
            break;
        case external:
            externalUse.clear();
            break;
        default:
            break;
    }
}

void Mesh::delCertainTriInalltriangles(Triangle* t) {

    eraseCertainTriangle(allTriangles,t);
}

void Mesh::delCertainEntryET(Triangle *t) {

    std::multimap<EdgeOrder, Triangle*>::iterator it;
    for(int i=0; i<3; i++){
        EdgeOrder oe = t->getEO(i);
        it = mmedgeTotriangles.find(oe);
        if(it == mmedgeTotriangles.end()) {
            std::cout<< "Not found " << oe << " " <<"in edge_to_triangle MMap " << std::endl;
            continue;
        }
        else{
            eraseCertainEntryET(mmedgeTotriangles, oe , t);
        }
    }
}

void Mesh::delCertainEntryPT(Triangle* t) {

    std::multimap<Point*, Triangle*,ComparePoint>::iterator it;
    for(int i=0; i<3; i++) {
        Point* p0 = t->getCorners(i);
        it = mmpointTotriangles.find(p0);
        if(it == mmpointTotriangles.end()) {
            std::cout<< "Not found " << *p0 << " " <<"in point_to_triangle MMap " << std::endl;
            continue;
        }
        else {
            eraseCertainEntryPT(mmpointTotriangles,p0,t);
        }
    }
}

Vector3 Mesh::getCentroid(const CentroidType ct) {

    switch (ct) {
        case CentroidType::vertex_based:
            return centroidVertex();
        case CentroidType::area_based:
            return centroidSurface();
        case CentroidType::volume_based:
            return centroidVolume();
        default:
            break;
    }
}

//used during edge collapse operation.
std::vector< std::pair<EdgeOrder,EdgeOrder> > Mesh::updateEdgeInfo(Triangle* To_change, Point* oldVertex, Point* newVertex) {

    int oldPoint_id = To_change->getVertexID(oldVertex);
    Point* p1 = To_change->getCorners(indexOrder_1(oldPoint_id));
    Point* p2 = To_change->getCorners(indexOrder_2(oldPoint_id));

    EdgeOrder ed1;
    EdgeOrder ed2;
    EdgeOrder ed1_new;
    EdgeOrder ed2_new;

    //Update Edge to triangle multimap.
    if (*p1 != *oldVertex) {
         ed1 =  EdgeOrder(p1,oldVertex);
         eraseCertainEntryET(mmedgeTotriangles,ed1,To_change);
    }
    if (*p2 != *oldVertex) {
        ed2 =  EdgeOrder(p2,oldVertex);
        eraseCertainEntryET(mmedgeTotriangles,ed2,To_change);
    }

    if (*p1 != *newVertex) {
        ed1_new =  EdgeOrder(p1,newVertex);
        mmedgeTotriangles.insert(std::make_pair(ed1_new,To_change));
    }
    if (*p2 != *newVertex) {
        ed2_new =  EdgeOrder(p2,newVertex);
        mmedgeTotriangles.insert(std::make_pair(ed2_new,To_change));
    }

    //Update point to triangle multimap.
    if (*oldVertex != *newVertex) {
        eraseCertainEntryPT(mmpointTotriangles,oldVertex,To_change);
    }
    if (*p1 != *newVertex && *p2 != *newVertex) {
        mmpointTotriangles.insert(std::make_pair(newVertex,To_change));
    }

    //Update triangle
    To_change->setNewVertex(newVertex,oldPoint_id);

    std::vector< std::pair<EdgeOrder,EdgeOrder> > old_to_new;
    old_to_new.emplace_back(std::make_pair(ed1,ed1_new));
    old_to_new.emplace_back(std::make_pair(ed2,ed2_new));
    return old_to_new;
}

void Mesh::getNeigTrianglesbyOrder(Triangle * t,  unsigned int &&order, std::vector<Triangle*> &TV) {
    //iterators
    TS_it it, itt;

    //Required Containers.
    std::set<Triangle*> visted;
    std::set<Triangle*> collected_triangles;
    std::set<Triangle*> temporary_triangles;
    collected_triangles.insert(t);

    for (int i = 0; i < order; i++) {
        temporary_triangles.clear();
        for (it = collected_triangles.begin(); it != collected_triangles.end(); it++) {
            itt = visted.find(*it);
            if (itt != visted.end()) continue;
            visted.insert(*it);
            Triangle *current_triangle(*it);
            for (int j = 0; j < 3; j++) {
                std::vector<Triangle *> Vectemp;
                EdgeOrder ed = current_triangle->getEO(j);
                std::pair<MMET, MMET> triangle_set = mmedgeTotriangles.equal_range(ed);
                if (std::distance(triangle_set.first, triangle_set.second) != 2) continue;

                for (MMET triangle_set_IT = triangle_set.first; triangle_set_IT != triangle_set.second; triangle_set_IT++) {
                    Triangle *tri = triangle_set_IT->second;
                    if (*tri == **it) continue;
                    Vectemp.push_back(triangle_set_IT->second);
                }
                if (Vectemp.size() == 1) {
                    temporary_triangles.insert(Vectemp[0]);
                }
            }
        }

//        for (auto InT = collected_triangles.begin(); InT != collected_triangles.end(); InT++) {
//            temporary_triangles.erase(*InT);
//        }
        for (auto newT = temporary_triangles.begin(); newT != temporary_triangles.end(); newT++) {
            collected_triangles.insert(*newT);
        }
    }
    TV.assign(collected_triangles.begin(), collected_triangles.end());
   //std::cout << "size of collected_triangles "<<collected_triangles.size()<<" "<<"vector_size "<<TV.size()<<std::endl;
}

void Mesh::getRingNeigbyOrder(Point *p, unsigned int &&order, std::vector<Triangle *> &TV) {
    //Required containers
    std::vector<Point*> collected_points, temp_collected_points;;
    std::set<Point*>  visted;
    std::set<Triangle*> collected_triangles;
    std::vector<Triangle*> vec_collected_triangle;
    //Required Iterators
    PV_it iP;
    TS_it iT;
    PS_it iiP;
    collected_points.push_back(p);

    for(int i=0; i<order; i++){
        vec_collected_triangle.clear();
        for(iP = collected_points.begin(); iP != collected_points.end(); iP++) {
            iiP = visted.find(*iP);
            if (iiP != visted.end()) continue;
            visted.insert(*iP);
            std::pair<MMPT, MMPT> triangle_set = mmpointTotriangles.equal_range(*iP);
            for (MMPT triangle_set_IT = triangle_set.first; triangle_set_IT != triangle_set.second; triangle_set_IT++) {
                collected_triangles.insert(triangle_set_IT->second);
                vec_collected_triangle.push_back(triangle_set_IT->second);
            }
        }

        std::vector<EdgeOrder> vedges;
        std::vector<Point*> bop ;
        getedgesByOrder(vec_collected_triangle,-1,vedges);
        getBorderPoints(vedges,bop);
        collected_points.clear();
        for(auto ele : bop) collected_points.push_back(ele);
    }

    TV.assign(collected_triangles.begin(),collected_triangles.end());
    //std::cout << "collected_triangles "<<collected_triangles.size()<<" "<<"TV "<<TV.size() << std::endl;

}

double Mesh::getMesh_SurfaceArea() {

    double total_area =0;
    std::vector<Triangle*> alltriangle;
    this->getTriangles(alltriangle);
    for(Triangle* t : alltriangle) {
        double area = t->getArea();
        total_area = total_area+area;
    }
    return total_area;
}

double Mesh::getMesh_Volume()  {

    if(!is_Solid(nullptr, nullptr)) {
        return 0;
    }

    std::vector<Point*> vertices;
    std::map<Point*, Vector3,ComparePoint> point_to_vector3;
    this->getVertices(vertices);
    Bbox_3 BB;
    for(Point* ele : vertices) {
        BB.add_coordinates(ele->x(),ele->y(),ele->z());
    }

    Vector3 min(BB.xmin(),BB.ymin(),BB.zmin());
    Vector3 max(BB.xmax(),BB.ymax(),BB.zmax());
    Vector3 refPoint = (min+max)/2;
    for(Point* p : vertices) {
        Vector3 dummy(p->x(),p->y(),p->z());
        dummy-refPoint;
        point_to_vector3.insert(std::make_pair(p,dummy));
    }

    double total=0;
    for(Triangle* t : allTriangles) {

        if(!t->isAlive) continue;
        Vector3 a = point_to_vector3[t->getCorners(0)];
        Vector3 b = point_to_vector3[t->getCorners(1)];
        Vector3 c = point_to_vector3[t->getCorners(2)];
        double tetVolume =  abs(dot(cross_product(a,b),c)/6.0);
        if(dot(a,t->getNormalVector()) > 0) {
            total = total+tetVolume;
        }
        else{
            total = total-tetVolume;
        }
    }
   // std::cout<<"total "<<total<<std::endl;
    return total;
}

Vector3 Mesh::centroidVertex()   {

    std::vector<Point*> vertices;
    this->getVertices(vertices);
    Vector3 centeroid_vertex(0,0,0);
    for(Point* p : vertices) {
        Vector3 temp (p->x(),p->y(),p->z());
        centeroid_vertex+temp;
    }
    centeroid_vertex/vertices.size();
    return centeroid_vertex;
}

Vector3 Mesh::centroidSurface()   {

    std::vector<Triangle*> triangles;
    this->getTriangles(triangles);
    Vector3 centeroid_surface(0,0,0);
    double total_area = 0;

    for(Triangle* t : triangles) {
        Point* mid = t->getCentroid();
        Vector3 dummy(mid->x(),mid->y(),mid->z());
        double current_area = t->getArea();
        dummy*current_area;
        centeroid_surface+dummy;
        total_area=total_area+current_area;
    }

    centeroid_surface/total_area;
    return centeroid_surface;
}

Vector3 Mesh::centroidVolume()   {

    std::vector<Point*> vertices;
    std::map<Point*, Vector3,ComparePoint> point_to_vector3;
    getVertices(vertices);
    Bbox_3 BB;
    for(Point* ele : vertices) {
        BB.add_coordinates(ele->x(),ele->y(),ele->z());
    }

    Vector3 min(BB.xmin(),BB.ymin(),BB.zmin());
    Vector3 max(BB.xmax(),BB.ymax(),BB.zmax());
    Vector3 refPoint = (min+max)/2;
    for(Point* p : vertices) {
        Vector3 dummy(p->x(),p->y(),p->z());
        dummy-refPoint;
        point_to_vector3.insert(std::make_pair(p,dummy));
    }

    std::map<int,std::pair<double, Vector3>> triID_to_centre_volume_pair;
    for(Triangle* t : allTriangles) {

        if(!t->isAlive) continue;
        Vector3 a = point_to_vector3[t->getCorners(0)];
        Vector3 b = point_to_vector3[t->getCorners(1)];
        Vector3 c = point_to_vector3[t->getCorners(2)];
        double tetVolume =  abs(dot(cross_product(a,b),c)/6.0);
        if(dot(a,t->getNormalVector()) < 0) {
            tetVolume *= -1;
        }
        Vector3 v0(t->getCorners(0)->x(),t->getCorners(0)->y(),t->getCorners(0)->z());
        Vector3 v1(t->getCorners(1)->x(),t->getCorners(1)->y(),t->getCorners(1)->z());
        Vector3 v2(t->getCorners(2)->x(),t->getCorners(2)->y(),t->getCorners(2)->z());
        (v0+v1+v2+refPoint)/4.0;
        triID_to_centre_volume_pair.insert(std::make_pair(t->ID,std::make_pair(tetVolume,v0)));
    }

    Vector3 centroid_volume(0,0,0);
    double totalvolume = 0;

    for(auto it = triID_to_centre_volume_pair.begin(); it != triID_to_centre_volume_pair.end(); it++) {

        double current_volume     = it->second.first;
        totalvolume               = totalvolume+current_volume;
        Vector3 tet_centroid      = it->second.second;
        tet_centroid*current_volume;
        centroid_volume+tet_centroid;
    }

    centroid_volume/totalvolume;
    return centroid_volume;
}

bool Mesh::is_Solid(std::vector<EdgeOrder>* border, std::vector<EdgeOrder>* nonmanifold)   {

    std::cout << "Collecting border and Non-manifold edges " << std::endl;
    bool ismanifold = true;
    std::vector<EdgeOrder> outtemp;
    std::set<EdgeOrder>alledges;
    std::vector<Triangle *>::const_iterator it;
    int border_count      = 0;
    int nonmanifold_count = 0;
    for (it = allTriangles.begin(); it != allTriangles.end(); it++) {
        for (int j = 0; j < 3; j++) {
            Triangle *tri(*it);
            Point *p0 = tri->getCorners(indexOrder_1(j));
            Point *p1 = tri->getCorners(indexOrder_2(j));
            EdgeOrder edge = EdgeOrder(p0, p1);
            outtemp.push_back(edge);
            alledges.insert(edge);
        }
    }

    std::sort(outtemp.begin(), outtemp.end());
    std::vector<EdgeOrder>::iterator iter;

    for (iter = outtemp.begin(); iter != outtemp.end(); iter++) {
        EdgeOrder EO = *iter;
        int count = 1;
        while (++iter != outtemp.end() && *iter == EO) {
            ++count;
        }
        --iter;
        if (count == 1) {
            if(border != nullptr){
                (*border).push_back(*iter);
            }
            ismanifold = false;
            ++border_count;
        } else if (count > 2) {
            if(nonmanifold != nullptr) {
                (*nonmanifold).push_back(*iter);
            }
            ismanifold = false;
            ++nonmanifold_count;
        }
    }
    if(!ismanifold) {
        std::cout << "Border_Edges: " << border_count << " " << "NonManifold_Edges: " << nonmanifold_count << std::endl;
    }
    return ismanifold;
}

void Mesh::establishEdgeinfo() {

    std::cout << "Establishing Edge informations " << std::endl;

    for(auto it = allTriangles.begin(); it != allTriangles.end(); it++) {
        Triangle* triangle(*it);
        for (int i=0; i<3; i++) {
            EdgeOrder Ed = triangle->getEO(i);
            std::pair<MMET, MMET> triangle_set = mmedgeTotriangles.equal_range(Ed);
            unsigned int distance = std::distance(triangle_set.first, triangle_set.second);
            EdgeOrder* pEdge = new EdgeOrder(Ed.p0,Ed.p1);
            if(distance == 1) {
                pEdge->status=EdgeStatus::Border;
                triangle->setEdge(pEdge,i);
            } else if (distance == 2) {
                pEdge->status=EdgeStatus::Manifold;
                triangle->setEdge(pEdge,i);
            }else if (distance > 2) {
                pEdge->status=EdgeStatus::Non_Manifold;
                triangle->setEdge(pEdge,i);
            }
        }
    }
}

bool Mesh::isNon_Manifold_Vertex(Point *input_vertex, std::vector<Triangle*> *ring_triangle ) {

        std::vector<Triangle*> one_ring;
        std::vector<EdgeOrder> B_Edges;
        getRingNeigbyOrder(input_vertex,1,one_ring);
        getedgesByOrder(one_ring,-1,B_Edges);
        if(ring_triangle != nullptr) {
            (*ring_triangle).assign(one_ring.begin(),one_ring.end());
        }
        InitilizeLoop obj;
        obj.createLoop(B_Edges);
        if (obj.loopCount() != 1) {
          //  std::cout<<"exist " << std::endl;
            return true;
        }
        else {
            return false;
        }
}

void Mesh::translateMesh(const Point *p) {

    std::vector<Point*> allpoints;
    this->getVertices(allpoints);
    Vector3 translate_vector = to_Vector3(p);
    std::map<Point*,Point*,ComparePoint> old_to_new_point;

    for(Point* old : allpoints) {
        Vector3 dummy = to_Vector3(old);
        dummy+translate_vector;
        Point* newpoint = to_Point(dummy);
      /*  double x_ = old->x()*1;
        double y_ = old->y()*1;
        double z_ = old->z()*0.9;
        Point* newpoint = new Point(x_,y_,z_);*/
        old_to_new_point.insert(std::make_pair(old,newpoint));
    }
    std::vector<Triangle*> alltriangle;
    this->getTriangles(alltriangle);

    for(auto t : alltriangle) {
        for(int i=0; i<3; i++) {
            Point* replace =  t->getCorners(i);
            Point* newpoint = old_to_new_point[replace];
            t->setNewVertex(newpoint,i);
        }
    }
}

void Mesh::standAlone(std::vector<Triangle*> &tv) {
    //std::cout << *allTriangles[0]->getCorners(0) << std::endl;
    getRingNeigbyOrder(allTriangles[0]->getCorners(0), 1, tv);
}

void Mesh::writeMeshSTL(std::string filename) {
    std::vector<Triangle*> outTri;
    this->getTriangles(outTri);
    writeSTL(filename+".stl",outTri);
}

void Mesh::clear() {

    allvertices.clear();
    vAllvertices.clear();
    mmpointTotriangles.clear();
    mmedgeTotriangles.clear();
    allTriangles.clear();
    externalUse.clear();
}

void Mesh::printContainersInfo() {

    std::cout << "Size of allvertices " << allvertices.size() << std::endl;
    std::cout << "Size of alltriangles " << allTriangles.size() << std::endl;
    std::cout << "size of point to triangle MMap " << mmpointTotriangles.size() << std::endl;
    std::cout << "size of edge to triangle Mmap " << mmedgeTotriangles.size() << std::endl;

}

