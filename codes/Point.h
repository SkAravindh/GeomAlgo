#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <tuple>
#include <memory>
#include "Vector_3.h"

class Mesh; //fwd
typedef Vector_3<double> Vector3;

class Point{
public:
    Point();
    Point(double x, double y, double z, Mesh* mesh);
    double x() const;
    double y() const;
    double z() const;
    Vector3 vertexNormal();
    bool operator < (const Point &rhs) const;
    bool operator > (const Point &rhs) const;
    bool operator == (const Point &rhs) const;
    bool operator != (const Point &rhs) const;
    Point* operator+ (const Point &rhs) const;
    Point* operator-(const Point& rhs) const;
    Point& operator = (const Point &rhs);


public:
    friend std::ostream & operator << (std::ostream &s, const Point &vertex) {
        s << vertex.x() << " " << vertex.y() << " " << vertex.z() << " ";
        return s;
    }

public:
    bool isAlive;
    unsigned int ID;
private:
    double coordinates[3];
    Mesh* powner;
};

struct ComparePoint {
    bool operator()(const Point* rhs, const Point* lhs) const {
        return *rhs < *lhs;
    }
};

#endif