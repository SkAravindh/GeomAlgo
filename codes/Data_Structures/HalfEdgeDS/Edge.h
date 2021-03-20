#ifndef EDGEHE_H
#define EDGEHE_H
#include "Vertex.h"

class Edge {

public:
    Edge() = default;

    Edge(Vertex *v1, Vertex *v2):v1(v1), v2(v2) {
    }

    void set_handle(unsigned int id){
        Edge_handle =id;
    }

    unsigned int get_handle() {
        return Edge_handle;
    }

public:
    Vertex* v1;
    Vertex* v2;
    HalfEdge* one_half_edge;
    bool isborder=false;

    friend std::ostream& operator<<(std::ostream&out, const Edge &ed ) {
        out <<"V1: " <<*(ed.v1)<<"; "<< " "<<"v2 " << *(ed.v2) << std::endl;
    }
private:
    unsigned int Edge_handle;
};

#endif