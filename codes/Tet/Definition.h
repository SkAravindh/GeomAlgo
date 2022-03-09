#ifndef DEFINITIOIN_H
#define DEFINITIOIN_H
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include "../Tet/Point3D.h"

class Mesh3D;


typedef Point3D<double, 3> Point_3;
typedef Point3D<double, 4> Point_4;
typedef std::shared_ptr<Mesh3D> TMeshptr;
typedef std::map<Point_3,Point_3*>::iterator Mpoint_3iter;


#endif
