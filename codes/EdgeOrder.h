#ifndef EDGEORDER_H
#define EDGEORDER_H
#include "Point.h"
class EdgeOrder {
public:
    EdgeOrder();
    EdgeOrder(Point *P0, Point *P1);
    bool operator <  (const EdgeOrder & rhs) const;
    bool operator == (const EdgeOrder & rhs) const;
    bool operator != (const EdgeOrder & rhs) const;


public:
    Point *p0;
    Point *p1;

    friend std::ostream & operator << (std::ostream & out, const EdgeOrder &rhs){
        out<<"OrderEdge "<<"P0 "<< *rhs.p0 <<" "<<"P1 "<< *rhs.p1 << " ";
        return out;
    }

};

struct CompareEdgeOrder{
    bool operator() (const EdgeOrder *lhs, const EdgeOrder *rhs)const {
        if(*(lhs->p0) < *(rhs->p0)) return true;
        if(*(lhs->p0) > *(rhs->p0)) return false;
        return (*(lhs->p1) < *(rhs->p1));
    }
};
#endif
