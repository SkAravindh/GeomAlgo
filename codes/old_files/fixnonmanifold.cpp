#include "../Mesh.h"


class Node {
public:
    Node()=default;
    Node(Triangle* input_tri);
    void collectedges();
    EdgeOrder getedge(int i);
    void determine_triangle();
    Triangle* getParenttriangle();
public:
    bool edge[3];
    bool tri_to_del=false;
private:
    Triangle* t;
    EdgeOrder edges[3];
};

Node::Node(Triangle* input_tri):t(input_tri) {}

void Node::collectedges() {

    for(int i=0; i<3; i++) {
        Point *p0 = t->getCorners(indexOrder_1(i));
        Point *p1 = t->getCorners(indexOrder_2(i));
        EdgeOrder Ed = EdgeOrder(p0, p1);
        edges[i] = Ed;
    }
}

Triangle* Node::getParenttriangle() {
    return t;
}

EdgeOrder Node::getedge(int i) {
    return edges[i];
}

void Node::determine_triangle() {

    int falseedge=0;
    for(int i=0; i<3; i++) {
        if(!edge[i]) {
            ++falseedge;
        }
    }
    if(falseedge >=2) {
        tri_to_del= true;
    }
}

class Fixnonmani {
public:
    Fixnonmani()=default;
    Fixnonmani(std::shared_ptr<Mesh> &pmesh);
    void fixTriangle();
private:
std::shared_ptr<Mesh> pMesh;
};
Fixnonmani::Fixnonmani(std::shared_ptr<Mesh> &pmesh): pMesh(pmesh){

}

void Fixnonmani::fixTriangle() {

    std::vector<Point*> allpoints;
    pMesh->getVertices(allpoints);

    for(auto ele : allpoints) {
        if(!ele->isAlive) continue;
        std::vector<Triangle*> ring_tri;
        bool re = pMesh->isNon_Manifold_Vertex(ele,&ring_tri);
        if(re) {

             std::vector<Node*> nodes;
             for(auto it = ring_tri.begin(); it != ring_tri.end(); it++) {
                 Node* obj = new Node(*it);
                 obj->collectedges();
                 nodes.push_back(obj);
             }

             for(auto iter = nodes.begin(); iter != nodes.end(); iter++) {
                 for (int i = 0; i < 3; i++) {
                     std::vector<Triangle*> neigs;
                     EdgeOrder ed = (*iter)->getedge(i);
                     pMesh->getAdjustenNeigh_1(ed,neigs);
                    // std::cout<<"size " <<neigs.size()<< std::endl;
                     if(neigs.size() !=2 ) {
                         std::cout<<"hi " << std::endl;
                         (*iter)->edge[i]=false;
                     }
                     else {
                         (*iter)->edge[i]=true;
                     }

                 }
                 (*iter)->determine_triangle();
             }
            for(auto ele : nodes) {
                if(ele->tri_to_del) {
                    std::cout<<"hello " << std::endl;
                    pMesh->delCertainEntryET(ele->getParenttriangle());
                    pMesh->delCertainEntryPT(ele->getParenttriangle());
                    ele->getParenttriangle()->isAlive= false;
                }
            }
        }
    }



   /* int count=0;
    for(auto ele : allpoints) {
        if (ele->isAlive) {
            ++count;
        }
    }
    std::cout<<"count " <<count<<" "<<allpoints.size()<< std::endl;*/
}
