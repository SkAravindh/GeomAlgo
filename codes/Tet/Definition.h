#ifndef DEFINITIOIN_H
#define DEFINITIOIN_H
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
//#include "../Tet/Point3D.h"
//#include "../Tet/Edge3D.h"

class Mesh3D;
template<typename T, int D> class Edge3D;
template<typename T, int D> class Point3D;
template<typename T, int D > class Face3D;
namespace TetElement
{
    class Tetrahedron;
}

typedef Point3D<double, 3> Point_3;
typedef Edge3D<Point_3*,2> EdgePts;
typedef Face3D<Point_3*,3> Face3;
typedef std::shared_ptr<Mesh3D> TMeshptr;
typedef std::map<Point_3,Point_3*>::iterator Mpoint_3iter;
typedef  std::vector<TetElement::Tetrahedron*> VTET;


#endif
