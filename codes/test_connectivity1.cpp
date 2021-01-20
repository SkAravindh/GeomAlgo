#include "Mesh.h"

//enum class TriPosition {
//    Interior, First, Second, Third, Parent
//};
//
//class SplitInfo {
//public:
//    SplitInfo () =default;
//    SplitInfo(Triangle* t);
//     void addT(Triangle* t, const int &tp);
//    SplitInfo* setParent (Triangle* t);
//    void setChild(std::vector<Triangle*> &ch);
//public:
//    Triangle* thistriangle= nullptr;
//    TriPosition triinfo;
//    std::vector<SplitInfo*> child;
//};
//
//SplitInfo* SplitInfo::setParent(Triangle *t) {
//
//    SplitInfo* obj = new SplitInfo();
//
//    obj->thistriangle = t;
//    obj->triinfo = TriPosition::Parent;
//    std::vector<Triangle*> children;
//    t->getChildren(children);
//    obj->setChild(children);
//    return obj;
//
//}
//
//void SplitInfo::setChild( std::vector<Triangle*> &ch) {
//
//    int count =0;
//    for (auto ele : ch ) {
//        this->addT(ele,count);
//        ++count;
//    }
//
//}
//
//void SplitInfo::addT(Triangle *t, const int &tp) {
//    SplitInfo* cobj = new SplitInfo();
//    cobj->thistriangle = t;
//    switch (tp) {
//        case 0:
//            cobj->triinfo=TriPosition::Interior;
//            break;
//        case 1:
//            cobj->triinfo=TriPosition::First;
//            break;
//        case 2:
//            cobj->triinfo=TriPosition::Second;
//            break;
//        case 3:
//            cobj->triinfo=TriPosition::Third;
//            break;
//        default:
//            break;
//    }
//   this->child.push_back(cobj);
//}

enum class SplitType {
    DivideTwo,
    DivideFour,
    Inner
};

class AdaptiveRefine {
public:
    AdaptiveRefine() = default;
    explicit AdaptiveRefine (std::shared_ptr<Mesh> &mesh);
    void faceRefinement();
    void splitIntoFour(Triangle* t);
    void splitIntoTwo(Triangle* t);
    SplitType greeORred(Triangle* t);
    void neighconnectivity(Triangle * p, Triangle * neigh);
    void deleteInfoFromDS(Triangle* T);
    void upDateDS(Triangle* T);
    void upDateDSInner(Triangle* T);

//

private:
    std::shared_ptr<Mesh> pMesh;
    std::vector<Triangle*> Track_Triangle;

};

AdaptiveRefine::AdaptiveRefine(std::shared_ptr<Mesh> &mesh) :pMesh(mesh) {
    pMesh->establishNeighcompleteMesh();
    std::cout << " after neigh establish-----------------------------------------------------------> " << std::endl<<std::endl;
}

void AdaptiveRefine::faceRefinement() {

    int count = 0;
    std::vector<Triangle*> triangle_details;
    pMesh->getTriangles(triangle_details);
    std::vector<Triangle*> Input_Triangle;
    Input_Triangle.push_back(triangle_details[0]);
    while (!Input_Triangle.empty()) {
        std::vector<Triangle*> parentTofefine;
        std::vector<Triangle*> child;

//        std::cout <<"Input_Triangle " <<Input_Triangle.size() << std::endl;
        for( auto ele : Input_Triangle ) {

            splitIntoFour(ele);
            parentTofefine.push_back(ele);
        }

        if (count == 1) {
            break;
        }
        ++count;

        for (auto par : parentTofefine) {
            par->getChildren(child);
        }
        Input_Triangle.clear();
        Input_Triangle.assign(child.begin(), child.end());


    }
}

void AdaptiveRefine::splitIntoTwo(Triangle *t) {

    std::vector<Triangle *> triangles_to_delete;
    for (int i = 0; i < 3; i++) {

        Triangle *NeighParent = t->getNeigh(i)->getParent();
        Triangle *Neigh = t->getNeigh(i);
        EdgeOrder Common = t->getEO(i);
//        if (Neigh->rt != RT::None) continue;
        triangles_to_delete.push_back(Neigh);
        int PeakVertexID = Neigh->getPeakVertexID(Common);
        Point *Midpoint = Common.getMidPoint();
        Point *Pvertex = Neigh->getCorners(PeakVertexID);
        Point *P1 = Neigh->getCorners(indexOrder_2(PeakVertexID));
        Point *P2 = Neigh->getCorners(indexOrder_1(PeakVertexID));
       Triangle* nk1 = pMesh->createTriangle(Midpoint, Pvertex, P1, Neigh);
        Triangle* nk2 = pMesh->createTriangle(Midpoint, P2, Pvertex, Neigh);
//        Neigh->rt = RT::Green;
//        nk1->rt = RT::None;
//        nk2->rt = RT::None;

        //  neighconnectivity(t,Neigh);
       deleteInfoFromDS(Neigh);
       upDateDS(Neigh);
    }

//    deleteInfoFromDS(t);
//    triangles_to_delete.push_back(t);
//    for (auto PC : triangles_to_delete) {
//        upDateDS(PC);
//    }

}

void AdaptiveRefine::splitIntoFour(Triangle *t) {

    //std::cout << "begin " << std::endl;
    Point* edge0mid = t->getEdgeMidPoint(0);
    Point* edge1mid = t->getEdgeMidPoint(1);
    Point* edge2mid = t->getEdgeMidPoint(2);

    //Splitting the parent triangle into four and storeing into alltriangle container(check Mesh.h)
    Triangle *Tri1 = pMesh->createTriangle(edge0mid,edge1mid,edge2mid,t);
    Triangle *Tri2 = pMesh->createTriangle(edge0mid,t->getCorners(2),edge1mid, t);
    Triangle *Tri3 = pMesh->createTriangle(edge1mid,t->getCorners(0),edge2mid, t);
    Triangle *Tri4 = pMesh->createTriangle(edge2mid,t->getCorners(1),edge0mid, t);
     //interior.
    //Interior_Triangle.insert(Tri1);
//    t->rt = RT::Red;
//    Tri1->rt = RT::None;
//    Tri1->Interior= true;
//    Tri2->rt = RT::None;
//    Tri3->rt = RT::None;
//    Tri4->rt = RT::None;

    SplitType obj = greeORred(t);

    if (obj == SplitType::DivideTwo) {
   //     splitIntoTwo(t);
        return;
    }

    if (obj == SplitType::DivideFour) {
        splitIntoFour(t);
        splitIntoTwo(t);
        return;
    }

}

SplitType AdaptiveRefine::greeORred(Triangle *t) {

//    if (t->Interior) {
//
//        std::cout << "inner   " << std::endl;
//        return SplitType::Inner;
//    }


    Triangle* Neg0 = t->getNeigh(0);
    Triangle* Neg1 = t->getNeigh(1);
    Triangle* Neg2 = t->getNeigh(2);

//    RT Nt0 = Neg0->rt;
//    RT Nt1 = Neg1->rt;
//    RT Nt2 = Neg2->rt;

//    if ( ( Nt0 == RT::None && Nt1 == RT::None) || ( Nt1 == RT::None && Nt2 == RT::None) || ( Nt2 == RT::None && Nt0 == RT::None) ) {
//        std::cout << "divide two " << std::endl;
//        return SplitType::DivideTwo;
//    }
//    else {
//        std::cout << "divide four  " << std::endl;
//    }


}

void AdaptiveRefine::neighconnectivity(Triangle *p, Triangle *neigh) {


    std::vector<Triangle*> child;
    neigh->getChildren(child);
    std::map<int, Triangle*> id_to_triangle;

    for (int i=0; i<3; i++) {
        Triangle* nt = neigh->getNeigh(i);
        if( *nt == *p) continue;
        id_to_triangle.insert(std::make_pair(i,nt));
    }

    for (auto ele : id_to_triangle) {
        EdgeOrder ed;
        int id = ele.second->getTriangleID(neigh);
        for (int i=0; i<child.size(); i++) {
            if (!getCommonEdge(ele.second, child[i], ed)) continue;
            ele.second->setNeigh(id,child[i]);
            break;
        }
    }
}

void AdaptiveRefine::deleteInfoFromDS(Triangle *T) {

    pMesh->delCertainEntryPT(T);
    pMesh->delCertainEntryET(T);
    pMesh->delCertainTriInalltriangles(T);
}

void AdaptiveRefine::upDateDS(Triangle *T) {
    std::vector<Triangle*> TV;
    T->getChildren(TV);

    for (auto ele : TV) {
        pMesh->establishNeighofTriangle(ele);
    }
    pMesh->fillTriangleContainers(TV,alltri);
}

void AdaptiveRefine::upDateDSInner(Triangle *T) {
    std::vector<Triangle*> TV;
    T->getChildren(TV);
    pMesh->fillTriangleContainers(TV,alltri);
}






//SplitType AdaptiveRefine::greeORred(Triangle *t) {
//
//    Triangle* tarray[3];
//    int tchildC[3];
//
//    for (int i=0; i<3; i++) {
//        Triangle* neigh = t->getNeigh(i)->getParent();
//        tarray[i]   = neigh;
//        tchildC[i]  = neigh->getChildSize();
//    }
//
//    int zerocount = 0;
//    int twocount=0;
//    for (int j=0; j<3; j++) {
//        if (tchildC[j] == 0 ) ++zerocount;
//        if (tchildC[j] == 2 ) ++twocount;
//    }
//
//    if (t->Tripos == TrianglePos::Interior ) {
//        std::cout << "inner " << std::endl;
//        return SplitType::Inner;
//    }
//
//    if (zerocount >= 2) {
//        std::cout << "divide two " << std::endl;
//        return SplitType::DivideTwo;
//    }
//
//    if (twocount >= 2) {
//        std::cout << "divide four " << std::endl;
//        return SplitType::DivideFour;
//    }
//
//
//}