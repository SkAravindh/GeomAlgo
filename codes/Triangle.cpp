#include "Triangle.h"



Triangle::Triangle(Point* P0, Point* P1, Point* P2, Mesh* mobj): powner(mobj) {
    Initilize(P0,P1,P2);
    powner->StoreTriangleInfo(this);
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

bool Triangle::operator == (const Triangle &rhs) const {
    return (*this->getCorners(0) == *rhs.getCorners(0) && *this->getCorners(1) == *rhs.getCorners(1) && *this->getCorners(2) == *rhs.getCorners(2));
}

bool Triangle::operator != (const Triangle &rhs) const {
    if(*this == rhs){
        return false;
    }
    else{
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

std::ostream & operator << (std::ostream &out, const Triangle &rhs){
    out<<"p0  "<<*rhs.getCorners(0)<<std::endl;
    out<<"p1  "<<*rhs.getCorners(1)<<std::endl;
    out<<"p2  "<<*rhs.getCorners(2)<<std::endl;
    return out;
}