#ifndef IOTET_H
#define IOTET_H
#include <fstream>
#include <sstream>
#include "../Tet/Definition.h"
#include "../Tet/Polygon3D.h"



TMeshptr ReadVtk(const std::string &filename);
template<typename T> std::vector<T> removeDupWord(std::string str, std::string vari);
void writeTetvtk(const std::string &filename, const std::vector<Point_3*> &vAllVertices, const std::vector<TetElement::Tetrahedron*> &vAllTetrahedron);
void writevtkPoints(const std::string& filename, std::vector<Point_3 > &bp);
#endif

