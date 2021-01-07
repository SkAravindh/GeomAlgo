#include "Triangle.h"

Triangle::Triangle() {
    corners[0] = nullptr;
    corners[1] = nullptr;
    corners[2] = nullptr;
}

Triangle::Triangle(Point* P0, Point* P1, Point* P2, Mesh* mobj): powner(mobj) {
    Initilize(P0,P1,P2);
    powner->storeTriangleInfo(this);
}

void Triangle::Initilize(Point *P0, Point *P1, Point *P2) {
    corners[0]=P0;
    corners[1]=P1;
    corners[2]=P2;

    neigb[0]= nullptr;
    neigb[1]= nullptr;
    neigb[2]= nullptr;
}

Point* Triangle::getCorners(int i) const {
    return corners[i];
}

EdgeOrder Triangle::getEO(int i) const {
    return EdgeOrder(corners[indexOrder_1(i)],corners[indexOrder_2(i)] );
    //return EdgeOrder(corners[(i+1)%3],corners[(i+2)%3] );
}

Edge Triangle::getEd(int i) const {
    return Edge(corners[indexOrder_1(i)],corners[indexOrder_2(i)] );
}

Vector3 Triangle::getNormalVector() const {
    Vector3 normal = getNormal(this);
    return normal;
}

Point* Triangle::getEdgeMidPoint(int i) const {
    Point *p0 = getCorners(indexOrder_1(i));
    Point *p1 = getCorners(indexOrder_2(i));
    Vector3 v0(p0->x(),p0->y(),p0->z());
    Vector3 v1(p1->x(),p1->y(),p1->z());
    Vector3 sum = v0+v1;
    Point* mid = new Point(sum.x()/2,sum.y()/2,sum.z()/2);
    return mid;
}

void Triangle::setNeigh(int i, Triangle *t) {
    neigb[i]=t;
}

Triangle* Triangle::getNeigh(int i) {
    return neigb[i];
}

void Triangle::addChild(Triangle *t) {
    t->glob=this;
    vchildren.push_back(t);
}

bool Triangle::haschild() {
    return !vchildren.empty();
}

void Triangle::clearVchild()  {
    vchildren.clear();
}

int Triangle::getVertexID(Point *p) {

    for(int i =0; i<3; i++) {
        if(*corners[i] == *p) return i;
    }
}

void Triangle::setNewVertex(Point *p, int idx) {
    corners[idx] = p;
}

void Triangle::getChildren(std::vector<Triangle *> &childvector) {

    if(!haschild()){
        childvector.push_back(this);
    } else{
        for(std::vector<Triangle*>::iterator it = vchildren.begin(); it != vchildren.end(); it++) {
            (*it)->getChildren(childvector);
        }
    }
}

int Triangle::getLongestEdgeID()   {

    double dis = -DBL_MAX;
    int idx;
    for(int i=0; i<3; i++) {
        EdgeOrder ed = this->getEO(i);
        double current_dis =  abs( getDistance(ed.p0,ed.p1) );
        if (current_dis > dis) {
            dis = current_dis;
            idx = i;
        //    std::cout << ed << "distance " << current_dis << " " << "id " << idx << std::endl;
        }
    }
    return idx;
}

int Triangle::getPeakVertexID(const EdgeOrder &c_ed) {

    for(int i=0; i<3; i++) {
        if (this->getEO(i) == c_ed) {
            return i;
        }
    }
}

bool Triangle::operator == (const Triangle &rhs) const {
    return (*this->getCorners(0) == *rhs.getCorners(0) && *this->getCorners(1) == *rhs.getCorners(1) && *this->getCorners(2) == *rhs.getCorners(2));
}

bool Triangle::operator != (const Triangle &rhs) const {

    if(*this == rhs) {
        return false;
    }
    else {
        return true;
    }
}

bool Triangle::operator < (const Triangle &rhs) const {
    //lexicographical operation.
    if( *this->getCorners(0) < *rhs.getCorners(0) ) return true;
    if( *this->getCorners(0) > *rhs.getCorners(0) ) return false;
    if( *this->getCorners(0) == *rhs.getCorners(0) ) {
        if( *this->getCorners(1) < *rhs.getCorners(1) ) return true;
        if( *this->getCorners(1) > *rhs.getCorners(1) ) return false;
        if( *this->getCorners(1) == *rhs.getCorners(1) ){
            if( *this->getCorners(2) < *rhs.getCorners(2) ) return true;
            if( *this->getCorners(2) > *rhs.getCorners(2) ) return false;
        }
    }
    return false;
}

std::ostream & operator << (std::ostream &out, const Triangle &rhs) {

    out<<"p0  "<<*rhs.getCorners(0)<<std::endl;
    out<<"p1  "<<*rhs.getCorners(1)<<std::endl;
    out<<"p2  "<<*rhs.getCorners(2)<<std::endl;
    return out;
}