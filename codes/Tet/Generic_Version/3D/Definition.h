#ifndef DEFINITIOIN_H
#define DEFINITIOIN_H
#include <memory>
#include <map>
#include <iostream>

class Mesh3D;
template<typename T, int D> class Point3D;
template<typename T, int D> class Edge3D;
template<typename T, typename Tobj> class Face3D;

typedef Point3D<double,3> Point_3;
typedef std::shared_ptr<Mesh3D> TMesh;
typedef Edge3D<Point_3*,2> Edge_3;
typedef std::map<Point_3,Point_3*>::iterator MPt3PtrIter;








#endif
