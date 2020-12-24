#ifndef WRITEDATA_H
#define WRITEDATA_H
#include <fstream>
#include "Mesh.h"
class Triangle;

void writePoints(std::string filename,std::vector<Point*> &bp);
void writeSTL(std::string filename , std::vector<Triangle*> &a);


#endif