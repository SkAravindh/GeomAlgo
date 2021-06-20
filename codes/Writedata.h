#ifndef WRITEDATA_H
#define WRITEDATA_H
#include <fstream>
#include <vector>
#include <memory>
#include "Triangle.h"
class Triangle;
class Mesh;




struct storevertex {

    storevertex() = default;
    void add(Point* p, int id) {
        point_to_id.push_back(std::make_pair(p,id));
    }
    int operator[] (Point* p) const {
        for(auto ele: point_to_id) {
            if(*ele.first==*p) {
                return ele.second;
            }
        }
    }
    int size () const {
        int size_ = point_to_id.size();
        return size_;
    }

    void print() {
        for(auto ele : point_to_id) {
            std::cout << *ele.first << " " <<ele.second << std::endl;
        }
    }

private:
   typedef std::pair<Point*, int> point_id_pair;
    std::vector<point_id_pair> point_to_id;

};

void writePoints(const std::string& filename,std::vector<Point*> &bp);
void writeSTL(const std::string& filename , std::vector<Triangle*> &a);
void writeVTK(const std::string& filename, std::shared_ptr<Mesh> &mesh);
void writequad(const std::string& filename, std::shared_ptr<Mesh> &mesh);

#endif