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
    Triangle* directNeigh_1;
    std::vector<Node*> children;
    bool isBoarder = false;
   std::set<EdgeOrder> Bedges;

};
Node::Node() {
    parent = nullptr;
    directNeigh_1 = nullptr;

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
    void splitIntoTwo(EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge, bool flag);
    void splitIntoThree( EdgeOrder &currLongEdge,  Triangle* CT,  Triangle *Neigh,  EdgeOrder &NeighLedge,bool flag);
    void avoidCOllision(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag);

private:
    std::shared_ptr<Mesh> pMesh;
    std::set<Point> borderPoints;
    std::set<Point>::iterator itr;
    std::set<EdgeOrder> borderEdges;
    typedef std::set<EdgeOrder>::iterator BEIT;
    std::vector<EdgeOrder> BED;
};

Refine::Refine() {
}

Refine::Refine(std::shared_ptr<Mesh> &mesh): pMesh(mesh) {}



void Refine::doRefine(int a) {
    std::vector<Triangle *> alltriangle;
    pMesh->getTriangles(alltriangle);
    std::vector<Triangle *> RingTri;
    Point *p = alltriangle[a]->getCorners(0);
   // pMesh->getRingNeigbyOrder(p, 3, RingTri);
   pMesh->getNeigTrianglesbyOrder(alltriangle[a],4,RingTri);
    writeSTL("tro.stl", RingTri);
    std::vector<EdgeOrder> boarderEd;
    getedgesByOrder(RingTri, -1, boarderEd);

    for (auto bp : boarderEd) {
        BED.push_back(bp);
    }
/*    std::vector<Point *> bpoints;
    getBorderPoints(boarderEd, bpoints);
    for (auto bp : bpoints) {
        borderPoints.insert(*bp);
    } */

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
   // std::cout <<"boarderEd size " << boarderEd.size() << std::endl;
    int countt=0;
    bool br = false;
    for (int i = 0; i <3; i++) {
        int countt=0;
        std::cout << "i is " << i << std::endl;
        //   writeSTL("ring.stl",RingTri);
        for (auto ele1 : triangleNode) {
            ++countt ;
         //   std::cout <<"countt " <<countt<< std::endl;
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
                    br = true;
//                    std::vector<Node *> ch;
//                    ele2->getNodeChild(ch);
//                    for (auto nc : ch) {
//                        std::cout << "inside assign " << std::endl;
//                        if (isBoarderT(nc)) {
//                        }
//                    }
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
  //  pMesh->reEstablishConnectivity();
}


bool Refine::isBoarderT( Node* cnode) {
   //  std::cout <<"borderPoints size " <<borderPoints.size()<< std::endl;
    Triangle *t = cnode->parent;
 //   std::cout <<"parent " <<*t << std::endl;
    std::set<EdgeOrder> be;
    for (int i=0; i<3; i++) {
        EdgeOrder ed = t->getEO(i);
        if (std::count(BED.begin(), BED.end(), ed)) {
            be.insert(ed);
        }
//        BEIT it = borderEdges.find(ed);
//        if( it != borderEdges.end()) {
//            be.push_back(ed);
//        }
    }
    if (be.empty()) {
        cnode->isBoarder= false;
        return false;
    }
    else {
        for (auto ele : be) {
            cnode->Bedges.insert(ele);
          //  std::cout << "inside assign ele " << ele << std::endl;
        }
        cnode->isBoarder= true;
      //  std::cout << "hi true " << std::endl;
        return true;
    }
 /*   for (int i = 0; i < 3; i++) {
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
        cnode->ed_1 = edge;
        cnode->isBoarder = true;
        return true;
    }
*/
  }

void Refine::splitType(Node *cnode) {
    Triangle* t= cnode->parent;
    std::vector<EdgeOrder> alledges;
    for (auto ele : cnode->Bedges) {
        alledges.push_back(ele);
    }

  //  std::cout <<"alledges " <<alledges.size() <<" " <<" cnode->Bedges "<< cnode->Bedges.size()<< std::endl;
    int count=0;
    bool flag = true;
    if(alledges.size() == 2) {
        flag = false;
    }

    for(auto ele : alledges) {
       // std::cout << "ele start  " <<ele << std::endl;
        ++count;
       // std::cout<<"count " <<count << std::endl;
        EdgeOrder ed = ele;
        Point* currentmid = ed.getMidPoint();
        EdgeOrder newed1(currentmid,ed.p0);
        EdgeOrder newed2(currentmid,ed.p1);
        BED.push_back(newed1);
        BED.push_back(newed2);
      //  borderEdges.insert(newed1);
       // borderEdges.insert(newed2);
        Triangle* direct_neigh ;
        EdgeOrder NeighT_Ledge;
        getNeighTriandLedge(ed,t,direct_neigh,NeighT_Ledge);
        HasCommonLE re = hassameedges(ed,NeighT_Ledge);

        if (re == HasCommonLE::same) {
              // std::cout << "two 1st" << std::endl;
            std::cout << "currLongEdge " <<ed<<std::endl;
            std::cout << "NeighLedge " <<NeighT_Ledge<<std::endl;
           splitIntoTwo(ed,t,direct_neigh,NeighT_Ledge,flag);
        }
        else if (re == HasCommonLE::not_same) {

            if (std::count(BED.begin(), BED.end(), NeighT_Ledge)) {
                std::cout << "three 1st" << std::endl;
                avoidCOllision(ed, t, direct_neigh, NeighT_Ledge, flag);
            } else {
                splitIntoThree(ed, t, direct_neigh, NeighT_Ledge, flag);
            }
        }

    }
   if(!flag) {
        deleteInfoFromDS(t);
    }
    /* EdgeOrder ed = cnode->ed_1;
    Triangle* direne = cnode->directNeigh_1;
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
*/
}

void Refine::avoidCOllision(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag) {
    Point* CurrentT_midpoint    =  currLongEdge.getMidPoint();
    int peak = Neigh->getPeakVertexID(currLongEdge);
    Point* vert = Neigh->getCorners(peak);
    Point* F_4 = Neigh->getCorners(indexOrder_2(peak));
    Point* F_5 = Neigh->getCorners(indexOrder_1(peak));
    pMesh->createTriangle(CurrentT_midpoint,vert,F_4,Neigh);
    pMesh->createTriangle(CurrentT_midpoint,F_5,vert,Neigh);
    if (flag) {
        deleteInfoFromDS(CT);
    }
    deleteInfoFromDS(Neigh);

}

void Refine::splitIntoTwo(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag) {
  //  std::cout<<"currLongEdge from two "<<currLongEdge<< std::endl;
    std::cout<<"CT from two "<<*CT<< std::endl;
    std::cout<<"CT from two "<<*Neigh<< std::endl;
    if (currLongEdge != NeighLedge) {
        std::cout << "currLongEdge " <<currLongEdge<<std::endl;
        std::cout << "NeighLedge " <<NeighLedge<<std::endl;
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
    if (flag) {
        deleteInfoFromDS(CT);
    }
    deleteInfoFromDS(Neigh);



}
void Refine::getNeighTriandLedge(const EdgeOrder &ed, const Triangle *CT, Triangle* &Neigh,
                                        EdgeOrder &NeighLedge) {

    std::vector<Triangle*> adjacent_triangles;
    pMesh->getAdjustenNeigh(ed,adjacent_triangles);

    if ( adjacent_triangles.size() != 2) {
        std::cout << "size is not 2 @checkForLongEdge" << std::endl;
        std::cout << "ed " <<ed << std::endl;
        std::cout << "ct " <<*CT << std::endl;
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

void Refine::splitIntoThree(EdgeOrder &currLongEdge, Triangle *CT, Triangle *Neigh, EdgeOrder &NeighLedge,bool flag) {
  //  std::cout<<"currLongEdge "<<currLongEdge<< std::endl;
 //   std::cout<<"CT "<<*CT<< std::endl;
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

    if(flag ) {
        deleteInfoFromDS(CT);
    }
    Triangle* second_direct_NeighT ;
    EdgeOrder second_NeighT_Ledge;

    getNeighTriandLedge(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge);
    HasCommonLE re = hassameedges(NeighLedge,second_NeighT_Ledge);

    if (re == HasCommonLE::same) {
     //   std::cout << "two 2st" << std::endl;
        splitIntoTwo(NeighLedge,Neigh,second_direct_NeighT,second_NeighT_Ledge, true);
    }
    else if (re == HasCommonLE::not_same) {
        //   std::cout << "three 2st" << std::endl;

        if (std::count(BED.begin(), BED.end(), second_NeighT_Ledge)) {
            std::cout << "redirect three 2nd" << std::endl;
            avoidCOllision(NeighLedge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
        } else {
            splitIntoThree(NeighLedge, Neigh, second_direct_NeighT, second_NeighT_Ledge, true);
        }
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

/*void Refine::splitneigh(Node *cnode) {
    Triangle* t= cnode->parent;
    std::vector<Triangle*> adjneigh;
    EdgeOrder ed = cnode->ed_1;
    pMesh->getAdjustenNeigh(ed,adjneigh);


    for(auto ele : adjneigh) {
        if (*ele == *t) continue;
        cnode->directNeigh_1=ele;
    }
    Triangle* direne = cnode->directNeigh_1;
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

}*/
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
