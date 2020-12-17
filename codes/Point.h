#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <tuple>


class Point{
public:
    Point();
    Point(double x, double y, double z);
    double x() const;
    double y() const;
    double z() const;
    bool operator < (const Point &rhs) const;
    bool operator > (const Point &rhs) const;
    bool operator == (const Point &rhs) const;
    bool operator != (const Point &rhs) const;
    Point& operator = (const Point &rhs);


public:
    friend std::ostream & operator << (std::ostream &s, const Point &vertex){
        s << vertex.x() << " " << vertex.y() << " " << vertex.z() << " ";
        return s;
    }

private:
    double coordinates[3];

};

struct ComparePoint{

    bool operator()(const Point* rhs, const Point* lhs) const {
        return *rhs < *lhs;
    }
};


#endif