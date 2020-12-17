#ifndef ERITEDATA_H
#define WRITESATA_H

#include <fstream>
#include "Mesh.h"

void writePoints(std::string filename,std::vector<Point*> &bp);
void writeSTL(std::string filename , std::vector<Triangle* > &a);


#endif