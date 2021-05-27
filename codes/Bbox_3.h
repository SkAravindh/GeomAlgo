#ifndef BBOX_3_H
#define BBOX_3_H
#include <cfloat>
#include <iostream>
#include "Point.h"

class Bbox_3{
public:
    Bbox_3();
    Bbox_3(double xmin, double ymin,double zmin, double xmax, double ymax ,double zmax);
    double xmin() const;
    double ymin() const;
    double zmin() const;
    double xmax() const;
    double ymax() const;
    double zmax() const;
    void getbounds(double &a_xmin, double &a_ymin, double &a_zmin,double &a_xmax, double &a_ymax, double &a_zmax) const;
    void add_coordinates(double x, double y, double z);
    void add_coordinates(double x, double y, double z, size_t id, Point* p);
    void getPointIDS(size_t* bounds,Point* coords[]);
    bool check_range(double Amin,double Amax,double Bmin,double Bmax);
    bool do_overlap(const Bbox_3 &lhs, const Bbox_3 &rhs);
    Bbox_3 operator +(const Bbox_3 &rhs) ;
    bool operator==(const Bbox_3 &rhs) const;
    friend std::ostream &operator << (std::ostream &out, const Bbox_3 &rhs);


private:
    double x_min, y_min, z_min;
    double x_max, y_max, z_max;
    size_t ids[6];
    Point* allpts[6];
};


#endif