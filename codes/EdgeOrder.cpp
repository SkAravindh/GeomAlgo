#include "EdgeOrder.h"

EdgeOrder::EdgeOrder() {
//    p0=nullptr;
//    p1= nullptr;
}

EdgeOrder::EdgeOrder(Point* P0, Point* P1) : p0(P0), p1(P1){
    if(*p0 > *p1){
        std::swap(p0,p1);
    }
}


Point* EdgeOrder::getMidPoint() const{

    Vector3 v0(this->p0->x(),this->p0->y(),this->p0->z());
    Vector3 v1(this->p1->x(),this->p1->y(),this->p1->z());
    Vector3 sum = v0+v1;
    Point* mid = new Point(sum.x()/2,sum.y()/2,sum.z()/2, nullptr);
    return mid;
}

bool EdgeOrder::operator <  (const EdgeOrder & rhs) const {

    if(*p0 < *rhs.p0) return true;
    if(*p0 > *rhs.p0) return false;
    return (*p1 < *rhs.p1);

}

double EdgeOrder::getlength() const {
    double a = (std::sqrt( std::pow((p1->x()-p0->x()),2) + std::pow((p1->y()-p0->y()),2) + std::pow((p1->z()-p0->z()),2)) );
    return abs(a);
}

bool EdgeOrder::operator == (const EdgeOrder &rhs) const{
    return ( (*p0 == *rhs.p0) && (*p1 == *rhs.p1));
}

bool EdgeOrder::operator!=(const EdgeOrder &rhs) const{
    return ( (*p0 != *rhs.p0) || (*p1 != *rhs.p1));
}

