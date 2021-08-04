#include "ExtractIntTriangle.h"

Face::Face(unsigned int a, unsigned int b) {
    F_ids[0] = a;
    F_ids[1] = b;
}

unsigned int Face::getFID(unsigned int i) {
    return F_ids[i];
}

ExtractIntTri::ExtractIntTri(std::shared_ptr<Mesh> &pmesh) : pMesh(pmesh){
}

void ExtractIntTri::run() {
    computeFeatureEdges();
    establishConnectivity();
}

void ExtractIntTri::computeFeatureEdges() {

    pMesh->getTriangles(alltri);
    std::vector<EdgeOrder> alled;
    std::vector<EdgeOrder> feature_edges;
    getEdgesofTrianlges(alltri, alled);
    std::vector<Triangle *> adj_tris;
    for (auto ed : alled) {
        pMesh->getAdjustenNeigh_1(ed, adj_tris);
        if (adj_tris.size() != 2) continue;
        Triangle *t1 = adj_tris.at(0);
        Triangle *t2 = adj_tris.at(1);
        double angle = (getAngleBtwVectors(t1->getNormalVector(), t2->getNormalVector()) * 180) / 3.14;
        if (angle < 130 && angle > 30) {
            feature_edges.push_back(ed);
        }
    }
    std::vector<Looping *> all_loops;
    InitilizeLoop lp;
    lp.createLoop(feature_edges);
    std::cout << "Number of loops: " << " " << lp.loopCount() << std::endl;
    lp.getLoopVector(all_loops);
    std::vector<std::vector<EdgeOrder>> sep_loops;
    int count = 0;
    //Manually choosing loops for further process.
    for (auto ele : all_loops) {
        //   std::cout << "count " << count << std::endl;
        if (!(count == 1 || count == 10 || count == 11 || count == 17)) {
            count++;
            continue;
        }
        count++;
        std::vector<Edge> fed;
        ele->getEdgevector(fed);
        //std::cout << fed.size() << std::endl;
        std::vector<EdgeOrder> temped;
        for (auto f_edge : fed) {
            EdgeOrder ed_new = EdgeOrder(f_edge.p0, f_edge.p1);
            temped.push_back(ed_new);
            std::vector<Triangle *> adjusten;
            pMesh->getAdjustenNeigh(ed_new, adjusten);
            edge_to_face.insert({ed_new, Face(adjusten[0]->ID, adjusten[1]->ID)});
        }
        sep_loops.push_back(temped);
    }
}

void ExtractIntTri::establishConnectivity() {

    for(auto ele : edge_to_face) {
        for(int i=0; i<2; i++) {
            Triangle* t1 = alltri.at(ele.second.getFID(i));
            t1->is_set = true;
            for(int j=0; j<3; j++) {
                EdgeOrder ed = t1->getEO(j);
                t1->setEdge(ed,j);
                if(ed==ele.first) {
                    t1->getfE(j).feature_edge= true;
                }
            }
        }
    }
    int c=0;
    int b=0;
    for(auto TRI : alltri) {
        b=b+1;
        if(TRI->is_set) {
            b=b-1;
            c=c+1;
            continue;
        }
        for(int i=0; i<3; i++) {
            EdgeOrder ed = TRI->getEO(i);
            TRI->setEdge(ed,i);
        }
    }
//    std::cout<<"b "<<b<<" "<<"c "<<c<<std::endl;
    std::vector<Triangle*> triout;
    try {
    unsigned int  Initial_tri_ID = findInitialTriangle();
    extractInteriorTriangles(Initial_tri_ID);
    }
    catch (const char* msg) {
        std::cout<< msg << std::endl;
        EXIT_FAILURE;
    }
}

unsigned int ExtractIntTri::findInitialTriangle() {

    int pos = 0;
    int neg = 0;
    bool found = false;
    std::cout<<"size of edge_to_face: "<<edge_to_face.size()<<std::endl;
    for(auto ele : edge_to_face) {
        std::vector<Triangle *> pair;
        for (int i = 0; i < 2; i++) {
            Triangle *T = alltri.at(ele.second.getFID(i));
            // std::cout << dot(to_Vector3(T->getCentroid()), T->getNormalVector()) << std::endl;
            pair.push_back(alltri.at(ele.second.getFID(i)));
        }

        int id[2];
        bool status[2];
        for (int j = 0; j < 2; j++) {
            Triangle *tri = pair.at(j);
            Vector3 normal = tri->getNormalVector();
            Vector3 centroid = to_Vector3(tri->getCentroid());
            Vector3 unit_normal = normal.make_unit_vector();
            Vector3 scale_unitnormal = 3 * unit_normal;
            Vector3 change = (centroid + scale_unitnormal);
            double drs = dot(change, scale_unitnormal);
            if (drs > 0) {
                id[j] = tri->ID;
                status[j] = false;
                pos = pos + 1;
            } else if (drs < 0) {
                id[j] = tri->ID;
                status[j] = true; //inside triangle
                neg = neg + 1;
            }
        }
        //        std::cout<<"status[0] "<<status[0]<<" "<<status[1]<<std::endl;
        if (status[0] && !status[1] || !status[0] && status[1]) {
            if (status[0]) {
                found = true;
                return id[0];
            } else if (status[1]) {
                found = true;
                return id[1];
            }
        }
        pair.clear();
    }
    if(!found) {
        throw "Fail to find an initial triangle";
    }
}

void ExtractIntTri::extractInteriorTriangles(unsigned int id) {

    int FEdge_count = 0;
    std::queue<Triangle*> inside_triangles;
    Triangle* initial_tri = alltri.at(id);
    inside_triangles.push(initial_tri);
    while (!inside_triangles.empty()) {
        Triangle* t = inside_triangles.front();
        t->is_inside= true;
        inside_triangles.pop();
        for(int i=0; i<3; i++) {
            EdgeOrder ed = t->getfE(i);
            if(ed.feature_edge) {
                FEdge_count=FEdge_count+1;
                edge_to_face.erase(ed);
                continue;
            }
            std::vector<Triangle*> neigh;
            pMesh->getAdjustenNeigh_1(ed,neigh);
            if(neigh.size() != 2) {
                std::cout<<"size is not two at compute_inside_triangle " <<std::endl;
                throw "size is not two ";
            }
            for(Triangle* tri_ele: neigh) {
                if(*tri_ele==*t || tri_ele->is_inside) continue;
                tri_ele->is_inside  = true;
                inside_triangles.push(tri_ele);
            }
        }
    }

    for(Triangle* CT : alltri){
        if(CT->is_inside){
            CT->isAlive= true;
        } else{
            CT->isAlive= false;
        }
    }
    if(!edge_to_face.empty()) {
        extractInteriorTriangles(findInitialTriangle());
    }
}