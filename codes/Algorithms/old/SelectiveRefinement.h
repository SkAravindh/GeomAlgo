#ifndef SELECTIVEREFINEMENT_H
#define SELECTIVEREFINEMENT_H
#include "AdaptiveRefinement_EdgeBased.h"

class Node {
public:
    Node();
    Node(Triangle* t);
    Node* insert(Triangle* t);
    void fillChild();
    void getNodeChild(std::vector<Node *> &childvector);

public:
    Triangle* parent;
    Triangle* directNeigh_1;
    std::vector<Node*> children;
    bool isBoarder = false;
    std::set<EdgeOrder> Bedges;
};

class SelectiveRefine : public AdaptRefineED {
public:
    SelectiveRefine()=default;
    explicit SelectiveRefine(std::shared_ptr<Mesh> &mesh);
    void createPatch(Triangle* T, Point* P, int order);
    void loadParameter(int refineLevel);
    void run();
    void doRefine();
    bool isBoarderT(Node* cnode);
    void splitType(Node *cnode);

private:
    int Iteration;
    std::vector<Triangle*> patch;

private:
    std::set<EdgeOrder> borderEdges;
    std::set<Point>::iterator itr;

    //Iterators
    typedef std::set<EdgeOrder>::iterator BEIT;
    typedef std::vector<EdgeOrder>::iterator VBE_IT;
    typedef std::vector<Triangle*>::iterator VT_IT;
    typedef std::vector<Node*>::iterator Node_IT;
};


#endif


