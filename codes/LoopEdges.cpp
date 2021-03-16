#include "Mesh.h"


class Looping {
public:
    Looping() = default;
    bool hasLoopCompleted();
    void addEdge(Edge &ed);
    void getEdgevector(std::vector<Edge> &loopedges) {
        loopedges.assign(edgevec.begin(),edgevec.end());
    }
private:
    std::vector<Edge> edgevec;
};

void Looping::addEdge(Edge &ed) {
    edgevec.push_back(ed);
}

bool Looping::hasLoopCompleted() {
    Point* start = edgevec.begin()->p0;
    Point* end = edgevec.back().p1;
    if(*start == *end) {
        return true;
    }
    else {
        return false;
    }
}

class InitilizeLoop{
public:
    InitilizeLoop() = default;
    void createLoop(std::vector<EdgeOrder> &Border_ed);
    bool getNextEdge(Edge &current, Edge &next, std::multimap<Point*, EdgeOrder,ComparePoint> &point_to_edge);
    void getLoopVector(std::vector<Looping*> &inputvec) {
        inputvec.assign(loopVec.begin(),loopVec.end());
    }
private:
    std::vector<Looping*> loopVec;
};

bool InitilizeLoop::getNextEdge(Edge &current, Edge &next, std::multimap<Point*, EdgeOrder,ComparePoint> &point_to_edge) {

    typedef std::multimap<Point*, EdgeOrder,ComparePoint>::iterator IT;
    Point *p0 = current.p0;
    Point *p1 = current.p1;
    std::pair<IT,IT> edges = point_to_edge.equal_range(p1);
    int distance = std::distance(edges.first,edges.second);
    if (distance !=2 ) {
        return false;
    }
    for(IT iter = edges.first; iter != edges.second; iter++) {
        Point* Nxt_p0 = iter->second.p0;
        Point* Nxt_p1 = iter->second.p1;
        if(*p1==*Nxt_p1) std::swap(Nxt_p0,Nxt_p1);
        if (!(*Nxt_p0==*p1)) return false;
        if(*Nxt_p1 == *p0) continue;
        next = Edge(Nxt_p0,Nxt_p1);
        return true;
    }
    return false;
}

void InitilizeLoop::createLoop(std::vector<EdgeOrder> &Border_ed) {

    std::multimap<Point*, EdgeOrder,ComparePoint> point_to_edge;
    for(auto iter = Border_ed.begin(); iter != Border_ed.end(); iter++) {
        point_to_edge.insert(std::make_pair(iter->p0,*iter));
        point_to_edge.insert(std::make_pair(iter->p1,*iter));
    }

    std::set<EdgeOrder> edge_dummy(Border_ed.begin(), Border_ed.end());
    int count=0;
    while(!edge_dummy.empty()) {

        EdgeOrder ed = *edge_dummy.begin();
        EdgeOrder initialEdge(ed);
        edge_dummy.erase(initialEdge);
        Looping* obj = new Looping;
        loopVec.push_back(obj);
        Edge curr(ed.p0,ed.p1);
        Edge next;
        obj->addEdge(curr);
        ++count;
        while (getNextEdge(curr,next,point_to_edge)) {
            obj->addEdge(next);
            curr=next;
            EdgeOrder edtodel(next.p0,next.p1);
            edge_dummy.erase(edtodel);
            if(obj->hasLoopCompleted()) {
                break;
            }
        }
    }
    std::cout<<"Number of loops: " <<count << std::endl;
}