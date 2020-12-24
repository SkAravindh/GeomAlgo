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
bool checkForHalfEdge(const Triangle *t1, const Triangle *t2);
bool checkCommonVertex(const Triangle *t1, const Triangle *t2);
void getedgesByOrder(const std::vector<Triangle* > &TV, const int &i, std::vector<EdgeOrder> &out);
void getBorderPoints(const std::vector<EdgeOrder> &BE, std::vector<Point*> &VP);
void eraseCertainTriangle(std::vector<Triangle* > &TV, Triangle *t);
void eraseCertainEntryPT(std::multimap<Point*, Triangle*,ComparePoint>&mm, Point *key, Triangle* t);
void eraseCertainEntryET(std::multimap<EdgeOrder, Triangle*>&mm, EdgeOrder &key, Triangle* t);
bool getCommonEdge(Triangle *t1, Triangle *t2,EdgeOrder &oe );
int getNonCommonPointsIDs(Triangle *t1, Triangle *t2, int Triangle_no );




#endif