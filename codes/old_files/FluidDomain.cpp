#include "../Mesh.h"
#include "../LoopEdges.cpp"



class Face {
public:
    Face(unsigned int a, unsigned int b) {
        F_ids[0] = a;
        F_ids[1] = b;
    }
    unsigned int getFID(unsigned int i) {
        return F_ids[i];
    }
public:
  unsigned int F_ids[2];
};

class Fluid_Domain {
public:
    Fluid_Domain()= default ;
    Fluid_Domain(std::shared_ptr<Mesh> &pmesh);
    void identify_holes();
    void establish_connectivity();
    unsigned int find_initial_tri();
    void compute_inside_triangle(unsigned int id);
    void fun();
private:
    std::shared_ptr<Mesh> pMesh;
    std::map<EdgeOrder, Face> edge_to_face;
    std::vector<Triangle *> alltri;
};
Fluid_Domain::Fluid_Domain(std::shared_ptr<Mesh> &pmesh) {
    pMesh=pmesh;
}

void Fluid_Domain::establish_connectivity() {

    for(auto ele : edge_to_face){
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
    unsigned int  Initial_tri_ID = find_initial_tri();
    compute_inside_triangle(Initial_tri_ID);
//    writeSTL("initial.stl",triout);
}

void Fluid_Domain::compute_inside_triangle(unsigned int id) {

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
            if(neigh.size() != 2){
                std::cout<<"size is not two at compute_inside_triangle " <<std::endl;
                throw "size is not two ";
            }
            for(Triangle* tri_ele: neigh){
                if(*tri_ele==*t || tri_ele->is_inside) continue;
                tri_ele->is_inside  = true;
                inside_triangles.push(tri_ele);
            }
        }
    }

    for(auto allt : alltri){
        if(allt->is_inside){
            allt->isAlive= true;
        } else{
            allt->isAlive= false;
        }
    }
    if(!edge_to_face.empty()) {
        compute_inside_triangle(find_initial_tri());
    }
 //   pMesh->writeMeshSTL("final_1");
 //std::cout<<"count is " <<FEdge_count<<"edge_to_face "<<edge_to_face.size()<<std::endl;
}

unsigned int Fluid_Domain::find_initial_tri() {

    int pos = 0;
    int neg = 0;
    bool found = false;
    std::cout<<"edge_to_face "<<edge_to_face.size()<<std::endl;
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
            //  std::cout<< "drs " <<drs<<std::endl;
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
        std::cout<<" Fail to find an initial triangle "<<std::endl;
        throw "Fail to find an initial triangle";
    }
//std::cout<<"pos " <<pos<<" "<<"neg "<<neg<<std::endl;
}

void Fluid_Domain::identify_holes() {

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
    std::cout << "loopCount " << " " << lp.loopCount() << std::endl;
    lp.getLoopVector(all_loops);
    std::vector<std::vector<EdgeOrder>> sep_loops;
    int count = 0;
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
        //}
        // break;
    }

}

/*
void Fluid_Domain::fun() {

    std::vector<int> fo;
    std::vector<int> no_fo;
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
    std::cout << "loopCount " << " " << lp.loopCount() << std::endl;
    lp.getLoopVector(all_loops);

    int count = 0;
    for(auto ele:all_loops) {
        bool found = false;

        std::vector<Edge> fed;
        ele->getEdgevector(fed);
        for(int u=0; u<1; u++) {
           Edge f_edge = fed.at(u);
            EdgeOrder ed_new = EdgeOrder(f_edge.p0, f_edge.p1);
            std::vector<Triangle *> tri_adj;
            pMesh->getAdjustenNeigh_1(ed_new,tri_adj);
            int id[2];
            bool status[2];
            for(int j=0; j<2; j++) {
                Triangle *tri = tri_adj.at(j);
                Vector3 normal = tri->getNormalVector();
                Vector3 centroid = to_Vector3(tri->getCentroid());
                Vector3 unit_normal = normal.make_unit_vector();
                Vector3 scale_unitnormal = 3 * unit_normal;
                Vector3 change = (centroid + scale_unitnormal);
                double drs = dot(change, scale_unitnormal);
                if (drs > 0) {
                    id[j] = tri->ID;
                    status[j] = false;
                    // pos = pos + 1;
                } else if (drs < 0) {
                    id[j] = tri->ID;
                    status[j] = true; //inside triangle
                    //   neg = neg + 1;
                }
            }
                if (status[0] && !status[1] || !status[0] && status[1]) {
                    if (status[0]) {
                        found = true;
                       // return id[0];
                    } else if (status[1]) {
                        found = true;
                      //  return id[1];
                    }
                }

                if(found) {
                    fo.push_back(count);
                }
                else{
                    no_fo.push_back(count);
                }
            }
        count = count+1;
        }

      for(auto fo_:fo){
          std::cout<<"found id " <<fo_<<std::endl;
      }
    for(auto nfo_:no_fo){
        std::cout<<"not found id " <<nfo_<<std::endl;
    }
    std::vector<Edge> edid;
   auto ele = all_loops.at(16);
   ele->getEdgevector(edid);
   std::vector<EdgeOrder>edor;
   for(auto ed : edid) {
       EdgeOrder obj = EdgeOrder(ed.p0,ed.p1);
       edor.push_back(obj);
   }
   std::vector<Point*> idpo;
    getBorderPoints(edor,idpo);
    writePoints("ids.vtk",idpo);
    }
    */

/*  std::vector<Point *> glob;
  for (int i = 0; i < sep_loops.size(); i++) {
      std::vector<EdgeOrder> be = sep_loops.at(i);
      std::vector<Point *> bp;
      getBorderPoints(be, bp);
//        for(auto bop : bp) {
//            glob.push_back(bop);
//        }

      writePoints("bp" + std::to_string(i) + ".vtk", bp);
  }*/
//  writePoints("allfpoints.vtk", glob);


    /* std::vector<Point *> glob;
     for (int i = 0; i < sep_loops.size(); i++) {
         std::vector<EdgeOrder> be = sep_loops.at(i);
         std::vector<Point *> bp;
         getBorderPoints(be, bp);

         writePoints("bp" + std::to_string(i) + ".vtk", bp);
     }
 }*/
    /*
    int positive = 0;
    int negative = 0;
    std::cout << "edge_to_face " << edge_to_face.size() << std::endl;
    std::vector<Triangle *> pair;
    for (auto ele : edge_to_face) {
        for (int i = 0; i < 2; i++) {
            Triangle *T = alltri.at(ele.second.getFID(i));
            std::cout << dot(to_Vector3(T->getCentroid()), T->getNormalVector()) << std::endl;
            pair.push_back(alltri.at(ele.second.getFID(i)));

             }
                 writeSTL("pair2.stl",pair);
               break;

}
            Point *cen = pair[0]->getCentroid();
            Vector3 nor = pair[0]->getNormalVector();

            //      std::cout << "nor " << nor << std::endl;
            //    std::cout << "cen " << *cen << std::endl;

            //   pMesh->getTriangles(alltri);
                  for (int i = 0; i < alltri.size(); i++) {

                       Triangle *tri = alltri.at(i);
                       Vector3 centriod = to_Vector3(tri->getCentroid());
                       Vector3 unitnormal = (tri->getNormalVector().make_unit_vector());
                       //  Vector3 fi = unitnormal*dot(to_Vector3(cen),unitnormal);
                       // std::cout<<"fi " <<fi<<std::endl;
                       //  std::cout << "unitnormal " << unitnormal << std::endl;
                       Vector3 scale_unitnormal = 5 * unitnormal;
                       // std::cout << "scale_unitnormal " << scale_unitnormal << std::endl;
                       Vector3 change = (centriod + scale_unitnormal);
                       double hello = dot(change, tri->getNormalVector());
                       if (hello < 0) {

                           tri->isAlive = true; //inside
                       } else {
                           tri->isAlive = false;
                       }
                   }
   // pMesh->writeMeshSTL("ithink");
//std::cout <<"dot " <<" "<<dot(to_Vector3(cen), pair[0]->getNormalVector()) << std::endl;
                 Vector3 movenor = to_Vector3(pair[0]->getCorners(0)) + nor;
            //      std::cout << "movenor " << movenor << std::endl;
           //       std::cout << "vertex_0 " << *pair[i]->getCorners(0) << std::endl;
                  Vector3 unitnormal = (pair[0]->getNormalVector().make_unit_vector());
                  //  Vector3 fi = unitnormal*dot(to_Vector3(cen),unitnormal);
                  // std::cout<<"fi " <<fi<<std::endl;
                    std::cout << "unitnormal " << unitnormal << std::endl;
                  Vector3 scale_unitnormal = 5 * unitnormal;
                  std::cout << "scale_unitnormal " << scale_unitnormal << std::endl;
                  Vector3 change = (to_Vector3(cen) + scale_unitnormal);
                  double hello = dot(change, pair[0]->getNormalVector());
                  if (hello < 0) {
                      //pair[0]->isAlive = true;
                      negative = negative + 1;
                  } else {
                      positive = positive + 1;
                      //pair[i]->isAlive = false;
                  }

                  std::cout << "change " << change << std::endl;
                  std::cout << "hello " << dot(change, pair[0]->getNormalVector()) << std::endl;
      //std::cout<<"dot " <<dot(to_Vector3(cen),unitnormal) <<std::endl;
      //pMesh->writeMeshSTL("inside");
              }
            // pair.clear();


            //   std::cout<<"negative "<<negative<<" "<<"pos "<<positive<<std::endl;

//}

*/

