#include "../Mesh.h"


class Node {
public:
    Node();
    Node(Triangle* t);
    Node* insert(Triangle* t);
    void fillchild();
    void getNodeChild(std::vector<Node *> &childvector);

public:
    Triangle* parent;
    Triangle* directNeigh;
    std::vector<Node*> children;
    bool isBoarder = false;
    EdgeOrder ed;
};
Node::Node() {
    parent = nullptr;
    directNeigh = nullptr;
  //  children.assign(4, nullptr);
}

Node::Node(Triangle* t) {
    parent = t;
}

Node* Node::insert(Triangle *t) {
    Node* obj = new Node(t);
    return  obj;
}

void Node::fillchild() {

    std::vector<Triangle*> child;
    parent->getChildren(child);
    if(child.empty()) {
        std::cout << "no child.. Error from FILLCHILD function Node class " << std::endl;
        return;
    }
    for(auto ele : child) {
        Node* nobj = insert(ele);
        children.push_back(nobj);
    }
}

void Node::getNodeChild(std::vector<Node *> &childvector) {

    if(children.empty()){
        childvector.push_back(this);
    } else{
        for(std::vector<Node*>::iterator it = children.begin(); it != children.end(); it++) {
            (*it)->getNodeChild(childvector);
        }
    }
}

enum class HasCommonLE {
    same,
    not_same
};

class Refine {
public:
    Refine();
    Refine(std::shared_ptr<Mesh> &mesh);
    void doRefine(int a);
    bool isBoarderT(Node* cnode);
    void splitneigh(Node* cnode);
    void deleteInfoFromDS(Triangle *T);
   void upDateDS(Triangle *T);
   void splitType(Node* cnode);
    void getNeighTriandLedge(const EdgeOrder &ed, const Triangle* CT, Triangle* & Neigh, EdgeOrder &NeighLedge);
    HasCommonLE hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2);
    void splitIntoTwo(EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge);

private:
    std::shared_ptr<Mesh> pMesh;
    std::set<Point> borderPoints;
    std::set<Point>::iterator itr;
};

Refine::Refine() {
}

Refine::Refine(std::shared_ptr<Mesh> &mesh): pMesh(mesh) {}

void Refine::doRefine(int a) {
    std::vector<Triangle *> alltriangle;
    pMesh->getTriangles(alltriangle);
    std::vector<Triangle *> RingTri;
    Point *p = alltriangle[a]->getCorners(0);
    pMesh->getRingNeigbyOrder(p, 1, RingTri);
   //pMesh->getNeigTrianglesbyOrder(alltriangle[0],2,RingTri);
   // writeSTL("tri.stl", RingTri);
    std::vector<EdgeOrder> boarderEd;
    getedgesByOrder(RingTri, -1, boarderEd);

    std::vector<Point *> bpoints;
    getBorderPoints(boarderEd, bpoints);
    for (auto bp : bpoints) {
        borderPoints.insert(*bp);
    }

    //    std::cout << "points " << bpoints.size() << std::endl;
//    std::cout << "points " << borderPoints.size() << std::endl;
    // writePoints("points1.vtk", bpoints);

    Node *Tobj = new Node();
    std::vector<Node *> triangleNode;
    int count=0;
    for (auto ele : RingTri ) {
        Node *nobj = Tobj->insert(ele);
            triangleNode.push_back(nobj);
    }
    for (int i = 0; i < 2; i++) {
        std::cout << "i is " << i << std::endl;
        //   writeSTL("ring.stl",RingTri);
        for (auto ele1 : triangleNode) {
            Node *lt(ele1);
            std::vector<Node *> nodechild;
            lt->getNodeChild(nodechild);
            for (auto ele2 : nodechild) {
                Triangle *CT = ele2->parent;
                int longestedge_id = CT->getLongestEdgeID();

                EdgeOrder edL1 = CT->getEO(longestedge_id);
                Point *midL = CT->getEdgeMidPoint(longestedge_id);
                Point *mid1 = CT->getEdgeMidPoint(indexOrder_1(longestedge_id));
                Point *mid2 = CT->getEdgeMidPoint(indexOrder_2(longestedge_id));

                int peakid = CT->getPeakVertexID(edL1);
                Point *peakvertex = CT->getCorners(peakid);
                Point *p0 = CT->getCorners(indexOrder_1(peakid));
                Point *p1 = CT->getCorners(indexOrder_2(peakid));

                pMesh->createTriangle(midL, p1, mid1, CT);
                pMesh->createTriangle(midL, mid1, peakvertex, CT);
                pMesh->createTriangle(peakvertex, mid2, midL, CT);
                pMesh->createTriangle(midL, mid2, p0, CT);
                ele2->fillchild();


                if (isBoarderT(ele2)) {

                  //  splitneigh(ele2);
                    splitType(ele2);
                    std::vector<Node *> ch;
                    ele2->getNodeChild(ch);
                    for (auto nc : ch) {
                        if (isBoarderT(nc)) {
                        }
                    }

                }
                else {
                    std::vector<Triangle*> neighC;
                    ele2->parent->getChildren(neighC);
                    pMesh->delCertainTriInalltriangles(ele2->parent);
                    pMesh->delCertainEntryET(ele2->parent);
                    pMesh->delCertainEntryPT(ele2->parent);
                  //  pMesh->delCertainTriInalltriangles(t);
                    pMesh->fillTriangleContainers(neighC, alltri);
                }

            }

        }
    }
}


bool Refine::isBoarderT( Node* cnode) {
    //  std::cout <<"borderPoints size " <<borderPoints.size()<< std::endl;
    Triangle *t = cnode->parent;
    //   std::cout << *t << std::endl;
    std::vector<int> ids;
    for (int i = 0; i < 3; i++) {
        Point *p = t->getCorners(i);
        itr = borderPoints.find(*p);
        if (itr != borderPoints.end()) {
            ids.push_back(i);
        }
    }
    if (ids.size() != 2) {

        cnode->isBoarder = false;
        return false;
    } else {

        Point *p1 = t->getCorners(ids[0]);
        Point *p2 = t->getCorners(ids[1]);
        EdgeOrder edge(p1, p2);
        cnode->ed = edge;
        cnode->isBoarder = true;
        return true;
    }
}

void Refine::splitType(Node *cnode) {
    Triangle* t= cnode->parent;
    EdgeOrder ed = cnode->ed;
    Triangle* direne = cnode->directNeigh;
    Point* commonedmid = ed.getMidPoint();
    borderPoints.insert(*commonedmid);
    Triangle* direct_neigh ;
    EdgeOrder NeighT_Ledge;
    getNeighTriandLedge(ed,t,direct_neigh,NeighT_Ledge);
    HasCommonLE re = hassameedges(ed,NeighT_Ledge);

    if (re == HasCommonLE::same) {
    //    std::cout << "two " << std::endl;
        splitIntoTwo(ed,t,direct_neigh,NeighT_Ledge);
    }
    else if (re == HasCommonLE::not_same) {
      //  std::cout << "three " << std::endl;
        splitIntoThree(ed,t,direct_neigh,NeighT_Ledge);
    }

}
void Refine::splitIntoTwo(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge) {

    if (currLongEdge != NeighLedge) {
        std::cout << "Error at SplitIntoTwo function" << std::endl; //should not happen.
        return;
    }

    Point* CurrentT_midpoint    =  currLongEdge.getMidPoint();
    Point* nT_midpoint          =  NeighLedge.getMidPoint();
    int NeighT_LedgeID          =  Neigh->getLongestEdgeID();
    Point* NeighT_peakvertex_ID =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = Neigh->getCorners(indexOrder_2(NeighT_LedgeID));
    Point* F_5 = Neigh->getCorners(indexOrder_1(NeighT_LedgeID));

    pMesh->createTriangle(CurrentT_midpoint,NeighT_peakvertex_ID,F_4,Neigh);
    pMesh->createTriangle(CurrentT_midpoint,F_5,NeighT_peakvertex_ID,Neigh);
    deleteInfoFromDS(CT);
    deleteInfoFromDS(Neigh);

}
void Refine::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh,
                                        EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh(ed,adjacent_triangles);

    if ( adjacent_triangles.size() != 2) {
        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
        return;
    }
    for(auto ele : adjacent_triangles) {
        if (*ele == *CT ) continue;
        Neigh=ele;
    }

    int nT_LedgeID     = Neigh->getLongestEdgeID();
    EdgeOrder nT_Ledge = Neigh->getEO(nT_LedgeID);
    NeighLedge         = nT_Ledge;

}

void Refine::splitIntoThree(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge) {

    Point* NeighT_LedgeMP     =  NeighLedge.getMidPoint();
    Point* CurrentT_LedgeMP   =  currLongEdge.getMidPoint();
    int NeighT_LedgeID        =  Neigh->getLongestEdgeID();
    Point* NeighT_peakVertex  =  Neigh->getCorners(NeighT_LedgeID);

    Point* F_4 = currLongEdge.p0;
    Point* F_5 = NeighLedge.p1;

    if (*currLongEdge.p0 == *NeighT_peakVertex) {
        F_4 = currLongEdge.p1;
    }

    if (*F_4 == *NeighLedge.p1) {
        F_5 = NeighLedge.p0;
    }

    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_peakVertex,NeighT_LedgeMP,Neigh);
    pMesh->createTriangle(CurrentT_LedgeMP,NeighT_LedgeMP, F_4,Neigh);
    pMesh->createTriangle(NeighT_LedgeMP,NeighT_peakVertex,F_5,Neigh);

    deleteInfoFromDS(CT);
    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge);
    HasCommonLE re = hassameedges(NeighLedge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
        splitIntoTwo(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge);
    }
    else if (re == HasCommonLE::not_same) {
        splitIntoThree(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge);
    }

}

HasCommonLE Refine::hassameedges(const EdgeOrder &ed1,const EdgeOrder &ed2) {

    if(ed1 == ed2 ) {
        return HasCommonLE::same;
    }
    else {
        return HasCommonLE::not_same;
    }

}

void Refine::splitneigh(Node *cnode) {
    Triangle* t= cnode->parent;
    std::vector<Triangle*> adjneigh;
    EdgeOrder ed = cnode->ed;
    pMesh->getAdjustenNeigh(ed,adjneigh);


    for(auto ele : adjneigh) {
        if (*ele == *t) continue;
        cnode->directNeigh=ele;
    }
    Triangle* direne = cnode->directNeigh;
    Point* commonedmid = ed.getMidPoint();
    borderPoints.insert(*commonedmid);
    int id = direne->getPeakVertexID(ed);
    Point* peakvertex = direne->getCorners(id);
    pMesh->createTriangle(commonedmid,ed.p0,peakvertex,direne);
    pMesh->createTriangle(commonedmid,peakvertex,ed.p1,direne);
    deleteInfoFromDS(direne);
    deleteInfoFromDS(t);
//    std::vector<Triangle*> neighC;
//   direne->getChildren(neighC);
//   t->getChildren(neighC);
//    pMesh->delCertainTriInalltriangles(direne);
//    pMesh->delCertainTriInalltriangles(t);
//    pMesh->fillTriangleContainers(neighC, alltri);
//    pMesh->delCertainTriInalltriangles(neighC[0]);
//    pMesh->delCertainTriInalltriangles(neighC[1]);

}
void Refine::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);
    upDateDS(T);


}

void Refine::upDateDS(Triangle *T) {

    std::vector<Triangle*> TV;
    T->getChildren(TV);

    for (auto ele : TV) {
     //   pMesh->establishNeighofTriangle(ele);
    }
    pMesh->fillTriangleContainers(TV,alltri);

}