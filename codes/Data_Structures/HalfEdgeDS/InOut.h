#ifndef INOUT_H
#define INOUT_H
#include <memory>
#include "Mesh.h"
#include "stl_reader.h"




std::shared_ptr<Mesh> ReadSTL(std::string filename);
std::shared_ptr<Mesh> InOutToMesh(std::string Filename,stl_reader::StlMesh<float, unsigned int> &mesh);



#endif