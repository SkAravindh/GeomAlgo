#ifndef ERITEDATA_H
#define WRITESATA_H

#include <fstream>
#include "Mesh.h"

void writePoint(std::vector<EdgeOrder> &oe);
void writeSTL(std::string filename , std::vector<Triangle* > &a);


#endif