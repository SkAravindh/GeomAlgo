#ifndef GEOM_SUPPORT_H
#define GEOM_SUPPORT_H
#include <algorithm>
#include "Vector_3.h"
#include "Mesh.h"
typedef Vector_3<double> Vector3;
class Triangle; //fwd



Vector3 getNormal(const Triangle *t);
double getShortestDistance( const Triangle *t, Point *p);
Vector3 getOrthogonalProjectionVector( const Triangle *t, Point *p);
int indexOrder_1(int n);
int indexOrder_2(int n);
bool brayCentric(const Triangle *t, Point *p);
bool checkHalfEdge(const Triangle *t1, const Triangle *t2);
bool checkCommonVertex(const Triangle *t1, const Triangle *t2);
void getedgesByOrder(const std::vector<Triangle* > &TV, const int &i, std::vector<EdgeOrder> &out);
void getBorderPoints(const std::vector<EdgeOrder> &BE, std::vector<Point*> &VP);




#endif