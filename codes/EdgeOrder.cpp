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

bool EdgeOrder::operator <  (const EdgeOrder & rhs) const {
    if(*p0 < *rhs.p0) return true;
    if(*p0 > *rhs.p0) return false;
    return (*p1 < *rhs.p1);
}

bool EdgeOrder::operator == (const EdgeOrder &rhs) const{
    return ( (*p0 == *rhs.p0) && (*p1 == *rhs.p1));
}

bool EdgeOrder::operator!=(const EdgeOrder &rhs) const{
    return ( (*p0 != *rhs.p0) || (*p1 != *rhs.p1));
}

