#include "SelectiveRefinement.h"

Node::Node() {
    parent = nullptr;
    directNeigh_1 = nullptr;
}

Node::Node(Triangle* t) {
    parent = t;
}

Node* Node::insert(Triangle *t) {
    Node* obj = new Node(t);
    return  obj;
}

void Node::fillChild() {

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
    }
    else {
        for(std::vector<Node*>::iterator it = children.begin(); it != children.end(); it++) {
            (*it)->getNodeChild(childvector);
        }
    }
}


SelectiveRefine::SelectiveRefine(std::shared_ptr<Mesh> &mesh) : AdaptRefineED(mesh) {

}

void SelectiveRefine::loadParameter(int refineLevel) {
    Iteration = refineLevel;
}

void SelectiveRefine::createPatch(Triangle *T, Point *P, int order) {

    if (T != nullptr && P != nullptr) {
        std::cout << "Either Point or Triangle Object should be nullptr !! " << std::endl;
        return;
    }
    else if (T == nullptr) {
        pMesh->getRingNeigbyOrder(P,order,patch);
    }
    else {
        pMesh->getNeigTrianglesbyOrder(T,order,patch);
    }

}

void SelectiveRefine::doRefine() {

    if (patch.empty()) {
        std::cout << "Input patch to refine is empty !! " << std::endl;
        return;
    }
    std::vector<EdgeOrder> boarderEd;
    getedgesByOrder(patch, -1, boarderEd);

    for(VBE_IT iter = boarderEd.begin(); iter != boarderEd.end(); iter++) {
        BED.push_back(*iter);
    }

    Node *Nobj = new Node();
    std::vector<Node*> triangleNode;

    for(VT_IT itr = patch.begin(); itr != patch.end(); itr++) {
        Node *nobj = Nobj->insert(*itr);
        triangleNode.push_back(nobj);
    }

    for (int i = 0; i <Iteration; i++) {
        std::cout << "Iteration:  " << i << std::endl;
        for (Node_IT N_iter = triangleNode.begin(); N_iter != triangleNode.end(); N_iter++) {
            Node* CNobj(*N_iter);
            std::vector<Node *> nodechild;
            CNobj->getNodeChild(nodechild);

            for (Node_IT C_iter = nodechild.begin(); C_iter != nodechild.end(); C_iter++) {
                Triangle* CT = (*C_iter)->parent;
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
                (*C_iter)->fillChild();

                if( isBoarderT(*C_iter) ) {
                    splitType(*C_iter);
                } else {
                    std::vector<Triangle*> neighC;
                    (*C_iter)->parent->getChildren(neighC);
                    pMesh->delCertainTriInalltriangles((*C_iter)->parent);
                    pMesh->delCertainEntryET((*C_iter)->parent);
                    pMesh->delCertainEntryPT((*C_iter)->parent);
                    pMesh->fillTriangleContainers(neighC, alltri);
                }
            }
        }
    }
}

bool SelectiveRefine::isBoarderT(Node *cnode) {

    Triangle *t = cnode->parent;
    std::set<EdgeOrder> bedge;

    for (int i=0; i<3; i++) {
        EdgeOrder ed = t->getEO(i);
        if (std::count(BED.begin(), BED.end(), ed)) {
            bedge.insert(ed);
        }
    }

    if (bedge.empty()) {
        cnode->isBoarder= false;
        return false;
    }
    else {
        for (auto ele : bedge) {
            cnode->Bedges.insert(ele);
        }
        cnode->isBoarder= true;
        return true;
    }
}

void SelectiveRefine::splitType(Node *cnode) {

    Triangle* t= cnode->parent;
    std::vector<EdgeOrder> alledges;
    for (auto edge : cnode->Bedges) {
        alledges.push_back(edge);
    }

    bool flag = true;
    if(alledges.size() == 2) {
        flag = false;
    }

    for(auto edgetorefine : alledges) {

        EdgeOrder ed = edgetorefine;
        Point* currentmid = ed.getMidPoint();
        EdgeOrder newed1(currentmid,ed.p0);
        EdgeOrder newed2(currentmid,ed.p1);
        BED.push_back(newed1);
        BED.push_back(newed2);
        Triangle* direct_neigh ;
        EdgeOrder NeighT_Ledge;
        getNeighTriandLedge(ed,t,direct_neigh,NeighT_Ledge);
        HasCommonLE re = hassameedges(ed,NeighT_Ledge);

        if (re == HasCommonLE::same) {
            splitIntoTwo(ed,t,direct_neigh,NeighT_Ledge,flag);
        }
        else if (re == HasCommonLE::not_same) {
            if (std::count(BED.begin(), BED.end(), NeighT_Ledge)) {
                avoidCOllision(ed, t, direct_neigh, NeighT_Ledge, flag);
            } else {
                splitIntoThree(ed, t, direct_neigh, NeighT_Ledge, flag);
            }
        }

    }
    if(!flag) {
        deleteInfoFromDS(t);
    }

}

void SelectiveRefine::run() {
    doRefine();
}
