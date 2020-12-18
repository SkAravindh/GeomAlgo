#include "AlgorithmInitialize.h"

AlgorithmInitialize::AlgorithmInitialize(std::string name,std::shared_ptr<Mesh> &Mesh) {
    AlgoName=name;
    pMesh=Mesh;
}