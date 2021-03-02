#ifndef ADAPTIVEREFINEMENT_EDGEBASED_H
#define ADAPTIVEREFINEMENT_EDGEBASED_H
#include "../Mesh.h"

enum class HasCommonLE {
    same,
    not_same
};

class AdaptRefineED {
public:
    AdaptRefineED() = default;
    explicit AdaptRefineED(std::shared_ptr<Mesh> &mesh);
    void loadParameters(Triangle* input_Triangle, int refine_Level);
    void run();
    void edgeRefinement();
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle* CT, Triangle* & Neigh, EdgeOrder &NeighLedge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge, bool flag);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge, bool flag);
    void avoidCOllision(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag);
    void deleteInfoFromDS(Triangle* T);
    void upDateDS(Triangle* T);

private:
    //Input parameters
    Triangle* inputTriangle;
    int refineCount=1;

private:
    std::set<Triangle*> parents;

protected:
    //iterators
    std::shared_ptr<Mesh> pMesh;
    std::vector<EdgeOrder> BED;
    typedef std::set<Triangle*>::iterator TS_it;
};


#endif