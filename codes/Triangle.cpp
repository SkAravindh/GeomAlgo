#include "Triangle.h"


Triangle::Triangle() {

}
Triangle::Triangle(Point* P0, Point* P1, Point* P2, Mesh* mobj): powner(mobj) {
    Initilize(P0,P1,P2);
    powner->StoreTriangleInfo(this);
}

void Triangle::Initilize(Point *P0, Point *P1, Point *P2) {
    corners[0]=P0;
    corners[1]=P1;
    corners[2]=P2;

    neigb[0]=NULL;
    neigb[1]=NULL;
    neigb[2]=NULL;
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

bool Triangle::operator==(const Triangle &rhs) const {
    std::cout << "triangle bool "<<std::endl;
    return (this->getCorners(0)==rhs.getCorners(0) && this->getCorners(1)==rhs.getCorners(1) && this->getCorners(2)==rhs.getCorners(2));
}

std::ostream & operator<<(std::ostream &out, const Triangle &rhs){
    out<<"p0  "<<*rhs.getCorners(0)<<std::endl;
    out<<"p1  "<<*rhs.getCorners(1)<<std::endl;
    out<<"p2  "<<*rhs.getCorners(2)<<std::endl;
    return out;
}