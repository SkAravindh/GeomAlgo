#ifndef EDGE_H
#define EDGE_H
#include "Point.h"

class Edge {
    public:
        Edge();
        Edge(Point* p0, Point* p1);
        bool operator <  (const Edge & rhs) const;
        bool operator == (const Edge & rhs) const;
        bool operator != (const Edge & rhs) const;


        friend std::ostream & operator << (std::ostream & out, const Edge &rhs){
            out<<"OrderEdge "<<"P0 "<< *rhs.p0 <<" "<<"P1 "<< *rhs.p1 << " ";
            return out;
        }


    public:
        Point* p0;
        Point* p1;
};

#endif