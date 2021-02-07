#include "../Mesh.h"


class LoopFinalize{
public:
    LoopFinalize()=default;
    void addEdge(Edge &ed);
    bool isClosed();

private:
    std::vector<Edge> VED;
};

void LoopFinalize::addEdge(Edge &ed) {
    VED.push_back(ed);
}

bool LoopFinalize::isClosed() {
    Edge first_ed = *VED.begin();
    Edge last_ed = VED.back();
    std::cout << *first_ed.p0 <<" " << " " <<*last_ed.p1 << std::endl;
    if(*first_ed.p0 == *last_ed.p1) {
        return true;
    }
    else {
        return false;
    }
}

class Loop {
public:
    Loop() = default;
    Loop(std::vector<EdgeOrder> &B_Edges);
    void CreateLoop(std::vector<EdgeOrder> &B_Edges);
    bool GetNextEdge(Edge &current, Edge &next, std::map<Point*, std::vector<EdgeOrder>, ComparePoint> &Point_to_edge);

private:
    LoopFinalize* LF;
    std::vector<LoopFinalize*> loopVector;

};

Loop::Loop(std::vector<EdgeOrder> &B_Edges) {
    CreateLoop(B_Edges);
}

bool Loop::GetNextEdge(Edge &current, Edge &next,
                       std::map<Point *, std::vector<EdgeOrder>, ComparePoint> &Point_to_edge) {

    Point* plast = current.p0;
    std::vector<EdgeOrder> edvector = Point_to_edge[plast];
    if (edvector.size() != 2 ) {

        std::cout << "size of the vector is != 2  @GetNextEdge " << std::endl;
        return false;
    }
    std::cout<<"current    "<<current<< std::endl;
    for(auto ele : edvector) {
        std::cout <<ele << std::endl;
        EdgeOrder nxt = ele;
        if(*current.p0 != *nxt.p0) {
            if(*current.p0 == *nxt.p1) {
                if(*current.p1 != *nxt.p0) {
                    next=Edge(nxt.p1,nxt.p0);
                    std::cout << "hi" << std::endl;
                    return true;
                }
            }
        }
        else {
            if(*current.p1 != *nxt.p1) {
                next=Edge(nxt.p0,nxt.p1);
                std::cout << "hi 2" << std::endl;
                return true;
            }
        }

    }
    return false;
}


void Loop::CreateLoop(std::vector<EdgeOrder> &B_Edges) {


    std::vector<EdgeOrder>::iterator it;
    std::map<Point*, std::vector<EdgeOrder>, ComparePoint> Point_to_edge;
    for(it = B_Edges.begin(); it!= B_Edges.end(); it++) {
        Point_to_edge[it->p0].emplace_back(it->p0,it->p1);
        Point_to_edge[it->p1].emplace_back(it->p0,it->p1);
    }

  //  std::set<EdgeOrder> Set_ed(B_Edges.begin(),B_Edges.end());
    std::vector<EdgeOrder> temp;
    temp.assign(B_Edges.begin(),B_Edges.end());
    while (!temp.empty()) {
        EdgeOrder ed = *temp.begin();
//        Set_ed.erase(ed);
        temp.erase(std::remove(temp.begin(), temp.end(), ed), temp.end());
        LoopFinalize* LF = new LoopFinalize();
        loopVector.push_back(LF);
        Edge Ored(ed.p0,ed.p1);
        LF->addEdge(Ored);
        Edge NextED;
        int count =1;
       while( GetNextEdge(Ored,NextED,Point_to_edge) ) {
           std::cout <<"count " <<count << std::endl;
            std::cout << "NextED "<<NextED<<std::endl;
//            break;
            ++count;
            LF->addEdge(NextED);
            EdgeOrder tempe(Ored.p0,Ored.p1);
          //  Set_ed.erase(EdgeOrder(Ored.p0,Ored.p1));
           temp.erase(std::remove(temp.begin(), temp.end(), tempe), temp.end());
            if( LF->isClosed()) {
                break;
            }
////            if(count == 10) {
////                break;
////            }
            std::cout <<"size " <<temp.size()<< std::endl;
            Ored = NextED;
          // break;
       }
        break;
//        std::cout <<Ored<<std::endl;
//        std::cout <<NextED<<std::endl;

    }





}