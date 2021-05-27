#include "ConvexHull_3D.h"

ConvexHull::ConvexHull(std::vector<Point*> &Pvec) {
    Input_points.assign(Pvec.begin(),Pvec.end());
    No_of_points = Pvec.size();
    std::shared_ptr<Mesh> pmeshout(new Mesh());
    pMesh = pmeshout;
    std::cout << "No_of_points "<<No_of_points<<std::endl;
    compute();
}

void ConvexHull::initilize0utpoints() {

    if(Input_points.empty()){
        throw "Input_points container is empty";
    }

    size_t count =0;
    for(auto it = Input_points.begin(); it != Input_points.end(); it++, count++) {
        (*it)->ID=count;
        outside_points.insert(count);
    }

}

void ConvexHull::compute() {

    createInitialSimplex();
    std::vector<Triangle*> faces;
    pMesh->getTriangles(faces);
    std::queue<Triangle*> faceq;
    std::queue<Triangle*> popq;
    EdgeOrder edes[3];
    Triangle* adj_tri[3];
    std::vector<EdgeOrder> horizonedges;
    std::vector<Triangle*> newfaces;
    std::vector<Triangle*> popedfaces;
    for(Triangle* t : faces) {
        faceq.push(t);
    }
    Triangle* start;
    Vector3 further_pts;
    while (!faceq.empty()) {
        start = faceq.front();
        faceq.pop();
        if(!start->isAlive) continue;
        if(!getFurtherPoint(start, further_pts)) continue;
        popq.push(start);
        while (!popq.empty()) {
            Triangle* pFace = popq.front();
            getEdgesandFaces(pFace,edes,adj_tri);
            popq.pop();
            popedfaces.push_back(pFace);
            if(pFace->has_Degenerated()) continue;

            for(int i=0; i<3; i++) {
                Triangle* curr_t = adj_tri[i];
                if(curr_t == nullptr) continue;
                if(curr_t->has_Degenerated()) continue;
                if(faceVisible(curr_t,further_pts)) {
                    popq.push(curr_t);
                } else{
                    horizonedges.push_back(edes[i]);
                }
            }
        }
        newfaces.clear();
        for(auto hr_ed : horizonedges) {
            Triangle* newt = pMesh->createTriangle(to_Point(further_pts),hr_ed.p0,hr_ed.p1, nullptr);
            if(faceVisible(newt,centre)) {
                newt->flip();
            }
            faceq.push(newt);
            newfaces.push_back(newt);
        }
        updateExterior(popedfaces,newfaces);
    }
}

void ConvexHull::updateExterior(std::vector<Triangle*> &popedfaces,std::vector<Triangle*> &newfaces) {

    std::vector<size_t> check;
    std::vector<size_t> remove;
    for(auto opts : outside_points) {
        for(auto fc : popedfaces) {
            if(fc->has_contained(Input_points.at(opts))) {
                check.push_back(opts);
                break;
            }
            Vector3 pt = to_Vector3(Input_points.at(opts));
            if(faceVisible(fc,pt)) {
                check.push_back(opts);
                break;
            }
        }
    }

    for(auto opts_S : check) {
        for(auto newf : newfaces) {
            Vector3 newp = to_Vector3(Input_points.at(opts_S));
            if(!faceVisible(newf,newp)) {
                remove.push_back(opts_S);
                break;
            }
        }
    }
    for(auto ele : remove) {
        outside_points.erase(ele);
    }
}

void ConvexHull::createInitialSimplex() {

    size_t bounds[6]; //{x_min,y_min,z_min,x_max,y_max,z_max}
    Point* pts[6];  //{x_min,y_min,z_min,x_max,y_max,z_max} points of bounds
    size_t best[4];

    if (Input_points.size() < 4) {
        throw "Input_point container has less than 4 points ";
    }

    Bbox_3 bb;
    for(auto it = Input_points.begin(); it != Input_points.end(); it++) {
        bb.add_coordinates((*it)->x(),(*it)->y(),(*it)->z(),(*it)->ID,*it);
    }
    bb.getPointIDS(bounds,pts);
    std::vector<Point*> allp;
    for(int i=0; i<6; i++) {
       // std::cout << *Input_points.at(bounds[i]) <<std::endl;
        allp.push_back(Input_points.at(bounds[i]));
        //std::cout<<*pts[i]<<std::endl;
    }
   // std::cout<<bb<<std::endl;
   // writePoints("bounds.vtk",allp);

    double max_dis = -DBL_MAX;
    size_t id0, id1;

    for(int i=0; i<6; i++) {
        for(int j=1+i; j<6; j++) {
            double temp_dis = abs(getDistance(Input_points.at(bounds[i]), Input_points.at(bounds[j])) );
            if(temp_dis > max_dis) {
                max_dis = temp_dis;
                id0 = bounds[i];
                id1 = bounds[j];
            }
        }
    }
    best[0] = id0;
    best[1] = id1;

    if(max_dis <=0) {
        throw "Failed to identify a point to construct tetrahedron";
    }
   // std::cout<<*Input_points.at(id0) <<" "<< *Input_points.at(id1) << std::endl;
    max_dis = -DBL_MAX;

    size_t ref_ID = best[0];
    size_t id2;
    Vector3 v1    = to_Vector3(Input_points.at(ref_ID));
    Vector3 v2    = to_Vector3(Input_points.at(best[1]));
    Vector3 uDir  = (v2-v1).make_unit_vector();

    for(int i = 0; i < No_of_points; i++) {
        Vector3 W = to_Vector3(Input_points.at(i))- to_Vector3(Input_points.at(ref_ID));
        Vector3 U = uDir * dot(uDir, W );
        Vector3 V = W-U;
        double temp_dis = abs(V.lenght());
        if(temp_dis > max_dis) {
            max_dis = temp_dis;
            id2 = i;
        }
    }

    best[2]=id2;

    if(max_dis <=0) {
        throw "Failed to identify a point to construct tetrahedron";
    }

    max_dis = -DBL_MAX;
    size_t id3;
    Vector3 v3 = to_Vector3(Input_points.at(best[2]));
    Vector3 tri_normal = cross_product(v2-v1,v3-v1).make_unit_vector();
    for(int i=0; i<No_of_points; i++) {
         double temp_dis = abs(dot(tri_normal, to_Vector3(Input_points.at(i))-v1));
        if(temp_dis > max_dis) {
             max_dis = temp_dis;
             id3 = i;
        }
    }

    best[3] = id3;
    if(max_dis <=0) {
        throw "Failed to identify a point to construct tetrahedron";
    }

    Vector3 mid(0,0,0);
    for(int i=0; i<4; i++) {
       mid = mid + to_Vector3( Input_points.at(best[i]) );
    }
    centre = mid/4;

    Triangle* tetra_faces[4];
    tetra_faces[0] = pMesh->createTriangle(Input_points.at(best[0]),Input_points.at(best[1]),Input_points.at(best[2]), nullptr);
    tetra_faces[1] = pMesh->createTriangle(Input_points.at(best[0]),Input_points.at(best[2]),Input_points.at(best[3]), nullptr);
    tetra_faces[2] = pMesh->createTriangle(Input_points.at(best[1]),Input_points.at(best[2]),Input_points.at(best[3]), nullptr);
    tetra_faces[3] = pMesh->createTriangle(Input_points.at(best[0]),Input_points.at(best[1]),Input_points.at(best[3]), nullptr);

  //  pMesh->writeMeshSTL("tetra");
    for(int i=0; i<4; i++) {
        if(tetra_faces[i]->has_Degenerated()) continue;
        if(faceVisible(tetra_faces[i],centre)) {
            tetra_faces[i]->flip();
        }
    }

    std::vector<size_t> remove_points;
    bool outside;
    for(auto opts : outside_points) {
        outside = false;
        for(int j=0; j<4; j++) {
            if( tetra_faces[j]->has_contained(Input_points.at(opts))) {
                remove_points.push_back(opts);
                Input_points.at(opts)->isAlive = false;
                outside = true;
                break;
            }
            Vector3 current_pt = to_Vector3(Input_points.at(opts));
            if(faceVisible(tetra_faces[j], current_pt) ) {
                outside = true;
                break;
            }
        }
        if(!outside) {
            Input_points.at(opts)->isAlive = false;
            remove_points.push_back(opts);
        }
    }

    for(auto id : remove_points) {
        //std::cout<<"id "<<id<<std::endl;
        outside_points.erase(id);
    }
   std::cout<<"remove_points " <<remove_points.size()<<std::endl;
    std::cout<<"outpoints " <<outside_points.size()<<std::endl;
 /*   std::vector<Point*> in_p;
    std::vector<Point*> ou_p;
    for(int i=0; i< Input_points.size(); i++) {
        if(!Input_points.at(i)->isAlive) {
            in_p.push_back(Input_points.at(i));
        }
        else {
            ou_p.push_back(Input_points.at(i));
        }
        //in_p.push_back(Input_points.at(remove_points.at(i)));
    }
    std::cout<<"remove_points " <<in_p.size()<<std::endl;
    std::cout<<"outpoints " <<ou_p.size()<<std::endl;*/
//    for(auto ele : outside_points) {
//        ou_p.push_back(Input_points.at(ele));
//    }
//    writePoints("inside.vtk", in_p);
  //  writePoints("outside.vtk",ou_p);
}

bool ConvexHull::faceVisible(Triangle* t, Vector3 &vec) {

    return (vec.is_valid()) && (facePlaneDistance(t, vec) > PLANE_DISTANCE);
}

float ConvexHull::facePlaneDistance(Triangle *t, Vector3 &vec) {

    Vector3 trianlge_normal = t->getNormalVector();
    float re = (float) dot( vec - to_Vector3(t->getCorners(0)) , trianlge_normal);
    return re;
}


bool ConvexHull::getFurtherPoint(Triangle *t, Vector3 &fp) {

    size_t index = -1;
    auto dis = (float) -DBL_MAX;
    for(auto ids : outside_points) {
        if(t->has_contained(Input_points.at(ids))) continue;
        Vector3 pt = to_Vector3(Input_points.at(ids));
        float re   = facePlaneDistance(t,pt);
        if(re > dis) {
            dis = re;
            fp = pt;
            index = ids;
        }
    }

    return index != -1;
}

void ConvexHull::getEdgesandFaces(Triangle* t,EdgeOrder ed[3], Triangle* adj[3]) {

    EdgeOrder ed_temp[3];
    Triangle* adj_temp[3]={nullptr, nullptr, nullptr};
    for(int i=0; i<3; i++) {
        std::vector<Triangle*> adjtri;
        EdgeOrder edge = t->getEO(i);
        ed_temp[i] =edge;
        pMesh->getAdjustenNeigh_1(edge, adjtri);
        if(adjtri.size() > 2 ) {
            std::cout<<"check at getEdgesandFaces function " << std::endl;
            throw "neighbour triangle is more than 2";
        }
        for(int j=0; j<adjtri.size(); j++){
            if(*adjtri.at(j)==*t) continue;
            adj_temp[i] = adjtri.at(j);
        }
    }
    for(int i=0; i<3; i++) {
        ed[i] = ed_temp[i];
        adj[i] = adj_temp[i];
    }
    pMesh->delCertainEntryPT(t);
    pMesh->delCertainEntryET(t);
    t->isAlive = false;
}