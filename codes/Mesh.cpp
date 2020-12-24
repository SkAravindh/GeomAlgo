#include "Mesh.h"


Mesh::Mesh() {

}

Mesh::Mesh(std::string Filename, size_t Poolsize) : ModelName(Filename), Poolsize(Poolsize) {

}

Point* Mesh::CreateVertex(  double x,   double y,   double z) {
    Point P(x,y,z);
    Point * Pptr = CreateVertex(P);
    return Pptr;
}

Point* Mesh::CreateVertex(Point &P) {

    std::map<Point, Point*>::iterator it=allvertices.find(P);
    if(it != this->allvertices.end()){
        return it->second;
    }
    else{
        Point * pobj = new Point(P.x(),P.y(),P.z());
        this->allvertices.insert(std::make_pair(*pobj,pobj));
        vAllvertices.push_back(pobj);
        ++Number_OF_Vertices;
        return pobj;
    }
}

Triangle* Mesh::CreateTriangle(Point *P0, Point *P1, Point *P2,Triangle* parent) {
    Triangle *newT = new Triangle(P0, P1, P2,this);
    if(parent == nullptr) {
        allTriangles.push_back(newT);
    }
    else{
        parent->addchild(newT);
    }

    return newT;
}

void Mesh::StoreTriangleInfo(Triangle *T) {

    Point* p0 = T->getCorners(0);
    Point* p1 = T->getCorners(1);
    Point* p2 = T->getCorners(2);

    mmpointTotriangles.insert(std::make_pair(p0,T));
    mmpointTotriangles.insert(std::make_pair(p1,T));
    mmpointTotriangles.insert(std::make_pair(p2,T));

    mmedgeTotriangles.insert(std::make_pair(T->getEO(0),T));
    mmedgeTotriangles.insert(std::make_pair(T->getEO(1),T));
    mmedgeTotriangles.insert(std::make_pair(T->getEO(2),T));

}

void Mesh::getAdjustenNeigh(const EdgeOrder &ed, std::vector<Triangle*> &tv){
    std::vector<Triangle*> temp;
    typedef std::multimap<EdgeOrder, Triangle *>::iterator MMET;
    std::pair<MMET, MMET> triangle_set = mmedgeTotriangles.equal_range(ed);
    if (std::distance(triangle_set.first, triangle_set.second) != 2){
        std::cout << std::distance(triangle_set.first, triangle_set.second) << std::endl;
        std::cout << "Returning empty vector " << std::endl;
        return;
    }
    for (MMET triangle_set_IT = triangle_set.first; triangle_set_IT != triangle_set.second; triangle_set_IT++) {
        temp.push_back(triangle_set_IT->second);
    }
    tv.assign(temp.begin(),temp.end());
}



void Mesh::establishNeighcompleteMesh(){
    std::vector<Triangle*>::iterator it;
    for(it = allTriangles.begin(); it != allTriangles.end(); it++){
        establishNeighofTriangle(*it);
    }
}

void Mesh::establishNeighofTriangle(Triangle* t) {
    typedef std::multimap<EdgeOrder, Triangle *>::iterator MMET;
    Triangle* currentT(t);
    for(int i=0; i<3; i++) {
        EdgeOrder ed = currentT->getEO(i);
        std::vector<Triangle*> adjT;
        getAdjustenNeigh(ed,adjT);
        for(auto ele : adjT){
            if(*ele == *currentT){
                continue;
            } else{
                currentT->setNeigh(i,ele);
            }
        }
    }

}

void Mesh::reEstablishConnectivity() {
    //Call only when you completely update alltriangle container.
    mmpointTotriangles.clear();
    mmedgeTotriangles.clear();
    std::vector<Triangle*>::iterator it;
    for(it=allTriangles.begin(); it != allTriangles.end(); it++){
        StoreTriangleInfo(*it);
    }
}

void Mesh::getTriangles(std::vector<Triangle*> &TV){
    std::vector<Triangle*>::iterator it;
    for(it=allTriangles.begin(); it!=allTriangles.end(); it++){
        Triangle * tri(*it);
        TV.push_back(tri);
    }
}


void Mesh::fillTriangleContainers(std::vector<Triangle*> &tv,VecContainerName VN ){
    switch (VN) {
        case alltri:
            for(auto ele : tv){
                allTriangles.push_back(ele);
            }
            break;
        case external:
            for(auto ele : tv){
                externalUse.push_back(ele);
            }
            break;
        default:
            break;
    }
}

void Mesh::getTriangleContainers(std::vector<Triangle*> &tv,VecContainerName VN ){
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

void Mesh::delCertainTrisInalltriangles(std::vector<Triangle*> &tv){
    std::vector<Triangle*>::iterator it;
    for(it=tv.begin(); it != tv.end(); it++){
        eraseCertainTriangle(allTriangles,*it);
    }
}

void Mesh::delCertainEntryET(Triangle *t) {
    std::multimap<EdgeOrder, Triangle*>::iterator it;
    for(int i=0; i<3; i++){
        EdgeOrder oe = t->getEO(i);
        it = mmedgeTotriangles.find(oe);
        if(it == mmedgeTotriangles.end()){
            std::cout<< "Not found " << oe << " " <<"in edge_to_triangle MMap " << std::endl;
            continue;
        }
        else{
            eraseCertainEntryET(mmedgeTotriangles, oe , t);
        }

    }

}

void Mesh::delCertainEntryPT(Triangle* t){
    std::multimap<Point*, Triangle*,ComparePoint>::iterator it;
    for(int i=0; i<3; i++){
        Point* p0 = t->getCorners(i);
        it = mmpointTotriangles.find(p0);
        if(it == mmpointTotriangles.end()){
            std::cout<< "Not found " << *p0 << " " <<"in point_to_triangle MMap " << std::endl;
            continue;
        }
        else{
            eraseCertainEntryPT(mmpointTotriangles,p0,t);
        }
    }
}


void Mesh::getNeigTrianglesbyOrder(Triangle * t,  unsigned int &&order, std::vector<Triangle*> &TV) {

    //iterators

    std::set<Triangle *>::iterator it, itt;
    typedef std::multimap<EdgeOrder, Triangle *>::iterator MMET;
    //Required Containers.
    std::set<Triangle *> visted;
    std::set<Triangle *> collected_triangles;
    std::set<Triangle *> temporary_triangles;
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
    std::vector<Point*>::iterator iP;
    std::set<Point*>::iterator iiP;
    std::set<Triangle*>::iterator iT;
    typedef std::multimap<Point*, Triangle*,ComparePoint>::iterator MMPT;
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


void Mesh::standAlone(std::vector<Triangle*> &tv) {
    //std::cout << *allTriangles[0]->getCorners(0) << std::endl;
    getRingNeigbyOrder(allTriangles[0]->getCorners(0), 1, tv);

}

void Mesh::writemesh(std::string filename) {
    writeSTL(filename,allTriangles);
}

void Mesh::printInfo(){
    std::cout << "Size of alltriangles " << allTriangles.size() << std::endl;
    std::cout << "size of point to triangle MMap " << mmpointTotriangles.size() << std::endl;
    std::cout << "size of edge to triangle Mmap " << mmedgeTotriangles.size() << std::endl;
}