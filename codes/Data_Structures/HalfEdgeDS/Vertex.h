#ifndef VERTEXHE_H
#define VERTEXHE_H
#include <iostream>
#include <unordered_map>
#include <vector>
class HalfEdge;

class Vertex {
public:
        Vertex() = default;
        Vertex(double x, double y, double z) {
            coordinates[0] = x;
            coordinates[1] = y;
            coordinates[2] = z;
        }

        double x () const {
            return coordinates[0];
        }

        double y () const {
            return coordinates[1];
        }

        double z () const {
            return coordinates[2];
        }

        void set_handle(unsigned int id) {
            vertex_handle = id;
        }

        unsigned int get_handle() {
            return vertex_handle;
        }

        void add_IncomingHE(unsigned int i) {
            incoming.push_back(i);
        }

        std::vector<unsigned int> get_IncomingHE() {
            return incoming;
        }

        void add_OutgoingHE(unsigned int i) {
            outgoing.push_back(i);
        }

        std::vector<unsigned int> get_OutgoingHE() {
            return outgoing;
        }
public:
    bool operator == (const Vertex& lhs) const{
        return ( (this->coordinates[0] == lhs.coordinates[0]) && (this->coordinates[1] == lhs.coordinates[1]) && (this->coordinates[2] == lhs.coordinates[2]) );
        }

        bool operator != (const Vertex& lhs) const{
            return ( (this->coordinates[0] != lhs.coordinates[0]) || (this->coordinates[1] != lhs.coordinates[1]) || (this->coordinates[2] != lhs.coordinates[2]) );
        }

    friend std::ostream &operator<<( std::ostream& out, const Vertex & v) {
            out <<"x "<<v.x()<<" "<<"y " << v.y() <<" "<<"z "<<v.z();
            return out;
            }

private:
    double coordinates[3];
    unsigned int vertex_handle;
private:
   std::vector<unsigned int> incoming;
   std::vector<unsigned int> outgoing;

};


#endif