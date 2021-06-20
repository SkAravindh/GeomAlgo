#ifndef COLLISION_H
#define COLLISION_H
#include <algorithm>
#include "Vector_3.h"
#include "Point.h"
#include "Triangle.h"
#include "Geom_Support.h"
#include "Bbox_3.h"

typedef Vector_3<double> vector3;


struct Ray{
    vector3 startpoint;
    vector3 rayDirection;
    double t;
};

struct Plane_3{
    vector3 normal;
    vector3 p;
};



bool MTRayTriangleIntersection(vector3 rayOrigin, vector3 rayVector, Triangle *T, vector3 & Intersecting_Point );
bool RayTriangleIntersection(vector3 rayOrigin, vector3 rayVector, Triangle *T, vector3 & Intersecting_Point );

//Functions to calculate Triangle-Triangle intersection.
bool TriangleTriangleIntersection(Triangle *T1,Triangle *T2);
void computeInterval(double PV0,double PV1,double PV2, double D0,double D1,double D2,double d0d1,double d0d2, std::vector<double> &t);
double computeScalarT(double PV0,double PV1, double D0,double D1);
void getMaximumIndex(vector3 &a, int &index);

//Functions to calculate Triangle-Box intersection.
bool TriangleBoxIntersection(Triangle *t, Bbox_3 &box);
bool PerformanceTest(vector3 axis, std::vector<vector3> vertex, std::vector<double> exd);
void GetMinMax(double po,double p1,double p2, double &pmin, double &pmax);
void GetMinMax(double &xmin, double &ymin, double &zmin, double &xmax, double &ymax, double &zmax, std::vector<vector3> Vertex_RE);

//Line Segment INtersection. Based on following tutorial------> https://www.youtube.com/watch?v=ELQG5OvmAE8
bool lineSegmentIntersection(Vector3 &a, Vector3 &b, Vector3 &c, Vector3 &d);
#endif