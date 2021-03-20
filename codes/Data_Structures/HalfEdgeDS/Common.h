#ifndef COMMON_H
#define COMMON_H
#include "Vertex.h"
#include "Face.h"
#include "HalfEdge.h"
#include "boost/functional/hash.hpp"

struct Vertex_Hash {

    std::size_t operator() (const Vertex* v) const {
        std::size_t seed = 0;
        boost::hash_combine(seed,boost::hash_value(v->x()));
        boost::hash_combine(seed,boost::hash_value(v->y()));
        boost::hash_combine(seed,boost::hash_value(v->z()));
        return seed;
       /* size_t a=  std::hash<double>() (v->x());
        size_t b =std::hash<double>() (v->y());
        size_t c =std::hash<double>() (v->z());
        return (a ^ b ^ c) ; */
    }
};

struct CompareVertex {
    bool operator() (const Vertex* v1, const Vertex* v2) const {
        return ( (v1->x() == v2->x()) && (v1->y() == v2->y()) && (v1->z() == v2->z()) );
    }
};

struct Face_Hash {

    std::size_t operator()(const Face* f) const {
        Vertex_Hash obj;
        std::size_t v1 = obj(f->getCorners(0));
        std::size_t v2 = obj(f->getCorners(1));
        std::size_t v3 = obj(f->getCorners(2));
        return (v1 ^ v2 ^ v3) ;
    }
};

struct CompareFace {
    bool operator()(const Face* f1, const Face* f2) const {
        return ( *(f1->getCorners(0)) == *(f2->getCorners(0)) && *(f1->getCorners(1)) == *(f2->getCorners(1)) && *(f1->getCorners(2)) == *(f2->getCorners(2)) );
    }
};

struct Edge_Hash {

    std::size_t operator()(const Edge* ed) const {
        Vertex_Hash obj;
        std::size_t v1 = obj(ed->v1);
        std::size_t v2 = obj(ed->v2);
        return (v1 ^ v2 );
    }
};

struct CompareEdge {
    bool operator()(const Edge* ed1, const Edge* ed2) const {
        return ( (*(ed1->v1) == *(ed2->v1)) && (*(ed1->v2) == *(ed2->v2)) );
    }
};


struct HalfEdge_Hash {

    std::size_t operator()(const HalfEdge* he) const {
        Vertex_Hash obj;
        std::size_t v1 = obj(he->v1());
        std::size_t v2 = obj(he->v2());
        return (v1 ^ v2 );
    }
};

struct CompareHalfEdge {
    bool operator()(const HalfEdge* he1, const HalfEdge* he2) const {
        return ( (*(he1->v1()) == *(he2->v1())) && (*(he1->v2()) == *(he2->v2())) );
    }
};
#endif