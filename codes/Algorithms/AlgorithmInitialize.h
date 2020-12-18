#ifndef ALGORITHMINITIALIZE_H
#define ALGORITHMINITIALIZE_H
#include "../Mesh.h"

class AlgorithmInitialize{
public:
    AlgorithmInitialize()=default;
    explicit AlgorithmInitialize(std::string name,std::shared_ptr<Mesh> &Mesh);



protected:
    std::shared_ptr<Mesh> pMesh;
    std::string AlgoName;
};

#endif