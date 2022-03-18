#ifndef TETIO_H
#define TETIO_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Definition.h"
class IOVtk
{
public:
    IOVtk();
    void storePoint(const std::vector<double> &vPoints);
    void addTetandQuadids(const std::vector<int> &vds);
    void storeTetandQuad(const std::vector<int> & vIds , const std::string &arg);
    void storePrism(const std::vector<int> & vId);
    void splitQuadTet(const std::vector<int> & vIds);
    TMesh buildMesh();
    void print();


private:
    std::vector<std::vector<double>> allpoints;
    std::vector<std::vector<int>> alltriangleIDS;
    std::vector<std::vector<int>> allquadIDS;
    std::vector<std::vector<int>> alltetrahedronIDS;
    std::vector<std::vector<int>> allprismIDS;
    std::vector<std::vector<int>> allhexIDS;
private:
    std::vector<std::vector<int>> quadtetids;
    TMesh pmesh;

};


TMesh ReadVtk(const std::string &filename);
template<typename T> std::vector<T> removeDupWord(std::string str, std::string vari);
void writevtkPoints(const std::string& filename, std::vector<Point_3 > &bp);

#endif