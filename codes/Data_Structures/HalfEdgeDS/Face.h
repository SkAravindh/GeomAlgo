#ifndef FACEHE_H
#define FACEHE_H

class Vertex;
class HalfEdge;

class Face {
public:
    Face() = default;
    Face(Vertex *v1, Vertex *v2, Vertex *v3) {
        corners[0] = v1;
        corners[1] = v2;
        corners[2] = v3;
    }

    Vertex* getCorners(unsigned int i) const {
        return corners[i];
    }

    void set_handle(unsigned int id) {
        face_handle = id;
    }

    unsigned int get_handle() {
        return face_handle;
    }

public:
    friend std::ostream & operator << (std::ostream &out, const Face &rhs) {
        out<<"p0  "<<*rhs.getCorners(0)<<std::endl;
        out<<"p1  "<<*rhs.getCorners(1)<<std::endl;
        out<<"p2  "<<*rhs.getCorners(2)<<std::endl;
        return out;
    }

    bool operator==(const Face &rhs) const {
        return ( this->corners[0]==rhs.getCorners(0) && this->corners[1]==rhs.getCorners(1) && this->corners[2]==rhs.getCorners(2) );
    }
private:
    Vertex* corners[3];
    unsigned int face_handle;
public:
    HalfEdge* HE = nullptr;
};





#endif
