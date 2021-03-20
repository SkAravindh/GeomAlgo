#ifndef HALFEDGE_H
#define HALFEDGE_H
#include "Vertex.h"
#include "Face.h"
#include "Edge.h"


class HalfEdge {

public:
    HalfEdge() = default;
    HalfEdge(Vertex* v1, Vertex* v2) : V1(v1), V2(v2) {
    }

    void set_handle(unsigned int id) {
        halfedge_handle = id;
    }

    unsigned int get_handle() {
        return halfedge_handle;
    }

    void set_Face(Face* f) {
        face=f;
    }

    Face* get_Face() {
        return face;
    }

    Vertex* v1() const {
        return V1;
    }

    Vertex* v2() const {
        return V2;
    }

    friend std::ostream& operator<<(std::ostream&out, const HalfEdge &ed ) {
        out <<"V1: " <<*(ed.V1)<<"; "<< " "<<"v2: " << *(ed.V2) << std::endl;
    }
public:
    HalfEdge* twin = nullptr;
    HalfEdge* next = nullptr;
    HalfEdge* prev = nullptr;
    Edge* parent = nullptr;
    Face* face = nullptr;
    Vertex* vertex = nullptr;
    bool isborder = false;

private:
    Vertex* V1;
    Vertex* V2;
    unsigned int halfedge_handle;
};



#endif