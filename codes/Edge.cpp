#include "Edge.h"

Edge::Edge() {
    p0 = nullptr;
    p1 = nullptr;
}

Edge::Edge(Point *p0, Point *p1) : p0(p0), p1(p1) {
}

bool Edge::operator <  (const Edge & rhs) const {

    if(*p0 < *rhs.p0) return true;
    if(*p0 > *rhs.p0) return false;
    return (*p1 < *rhs.p1);

}

bool Edge::operator == (const Edge &rhs) const {
    return ( (*p0 == *rhs.p0) && (*p1 == *rhs.p1));
}

bool Edge::operator!=(const Edge &rhs) const {
    return ( (*p0 != *rhs.p0) || (*p1 != *rhs.p1));
}
