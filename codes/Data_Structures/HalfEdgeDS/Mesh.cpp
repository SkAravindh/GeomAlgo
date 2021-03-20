#include "Mesh.h"

Vertex* Mesh::createVertex(double x, double y, double z) {
    Vertex* v = new Vertex(x,y,z);
    return v;
}

Vertex* Mesh::addVertex(Vertex *v) {

    unsigned int vertex_id = 0;
    if(Vertex_to_Vertexhandle_Map.find(v) == Vertex_to_Vertexhandle_Map.end()) {
        if (!allVertex.empty()) {
            vertex_id = allVertex.size();
        }
        allVertex.push_back(v);
        v->set_handle(vertex_id);
        Vertex_to_Vertexhandle_Map.insert(std::make_pair(v,vertex_id));
        Vertexhandle_to_Vertex_Map.insert(std::make_pair(vertex_id,v));
    } else{
        vertex_id = v->get_handle();
        //  std::cout << "Vertex already exist with ID: "<<vertex_id<< std::endl;
        return Vertexhandle_to_Vertex_Map[vertex_id];
    }
  //  std::cout << "Vertex successfully added with ID: "<<vertex_id<< std::endl;
    return  Vertexhandle_to_Vertex_Map[vertex_id];
}

Face* Mesh::addFace(Vertex *v1, Vertex *v2, Vertex *v3) {
  //  std::cout << "added face " << std::endl;
    unsigned int face_id = 0;
    Face* f = new Face(v1,v2,v3);
    if(Face_to_Facehandle_Map.find(f) == Face_to_Facehandle_Map.end()) {
        if(!allFaces.empty()) {
            face_id = allFaces.size();
        }
        f->set_handle(face_id);
        allFaces.push_back(f);
        Face_to_Facehandle_Map.insert(std::make_pair(f,face_id));
        Facehandle_to_Face_Map.insert(std::make_pair(face_id,f));

        Edge* E1 = addEdge(v1,v2,f);
        Edge* E2 = addEdge(v2,v3,f);
        Edge* E3 = addEdge(v3,v1,f);
    } else {
        face_id = f->get_handle();
     //   std::cout << "Face already exist with ID: "<<face_id << std::endl;
        return Facehandle_to_Face_Map[face_id];
    }
  //  std::cout<< "Face Successfully added with ID: " << face_id << std::endl;
    return Facehandle_to_Face_Map[face_id];
}

Edge* Mesh::addEdge(Vertex *v1, Vertex *v2, Face* f) {

    unsigned int edge_id=0;
    Edge* ed_current = new Edge(v1,v2);
    if(Edge_to_Edgehandle.find(ed_current) == Edge_to_Edgehandle.end()) {
        Edge* ed_opposite = new Edge(v2,v1);
      //  std::cout<< ed_opposite->get_handle() << std::endl;
        if(Edge_to_Edgehandle.find(ed_opposite) == Edge_to_Edgehandle.end()) {
            if(!allEdges.empty()) {
                edge_id = allEdges.size();
            }
            ed_current->set_handle(edge_id);
            allEdges.push_back(ed_current);
            Edge_to_Edgehandle.insert(std::make_pair(ed_current,edge_id));
            Edgehandle_to_Edge_map.insert(std::make_pair(edge_id,ed_current));
            HalfEdge* he = addHalfEdge(v1,v2,f);
            he->parent = ed_current;
            Edge_to_oneHalfEdge[ed_current] = he;
        } else {
            HalfEdge* he = addHalfEdge(v1,v2,f);
            edge_id = Edge_to_Edgehandle[ed_opposite];
            he->parent=ed_opposite;
     //       std::cout << "Edge already exist with ID fi: " << edge_id << std::endl;
            //std::cout << * Edgehandle_to_Edge_map[edge_id] << std::endl;
            Edge_to_oneHalfEdge[ed_opposite] = he;
            return Edgehandle_to_Edge_map[edge_id];
        }
    }else{
        edge_id = Edge_to_Edgehandle[ed_current];
     //   std::cout << "Edge already exist with ID se: "<< edge_id << std::endl;
        return  Edgehandle_to_Edge_map[edge_id];
    }
  //  std::cout << "Edge added successfully with ID: "<< edge_id << std::endl;
    //std::cout << * Edgehandle_to_Edge_map[edge_id] << std::endl;
    return  Edgehandle_to_Edge_map[edge_id];
}

HalfEdge* Mesh::addHalfEdge(Vertex *v1, Vertex *v2, Face *f) {

    unsigned int halfedge_id =0;
    HalfEdge* new_he = new HalfEdge(v1,v2);
    if(HalfEdge_to_HalfEdgehandle.find(new_he) == HalfEdge_to_HalfEdgehandle.end()) {
        if(!allHalfEdges.empty()) {
            halfedge_id = allHalfEdges.size();
        }
        new_he->set_handle(halfedge_id);
        new_he->set_Face(f);
        allHalfEdges.push_back(new_he);
        HalfEdge_to_HalfEdgehandle.insert(std::make_pair(new_he,halfedge_id));
        HalfEdgehandle_to_HEdge_map.insert(std::make_pair(halfedge_id,new_he));
        HalfEdge* twin_he = new HalfEdge(v2,v1);
        if(HalfEdge_to_HalfEdgehandle.find(twin_he) != HalfEdge_to_HalfEdgehandle.end() ) {
            new_he->twin = twin_he;
            twin_he->twin = new_he;
        }
        v1->add_OutgoingHE(halfedge_id);
        v2->add_IncomingHE(halfedge_id);

        Face_to_Halfedge_Map.insert(std::make_pair(f,new_he));
     //   std::cout << "HalfEdge is added successfully with ID: " << halfedge_id << std::endl;
    } else {
        halfedge_id = HalfEdge_to_HalfEdgehandle[new_he];
     //   std::cout << "HalfEdge is exist with a ID: " << halfedge_id << std::endl;
    }
    return HalfEdgehandle_to_HEdge_map[halfedge_id];
}

void Mesh::establishconnectivity() {

    for(auto it = Face_to_Halfedge_Map.begin(); it != Face_to_Halfedge_Map.end(); it++) {
        Face* face = it->first;
        face->HE = it->second;
    }

    for(auto it = Edge_to_oneHalfEdge.begin(); it != Edge_to_oneHalfEdge.end(); it++) {
        Edge* ed = it->first;
        HalfEdge* he = it->second;
        ed->one_half_edge = he;
    }

    for(auto it = allEdges.begin(); it != allEdges.end(); it++) {
        Edge* ed(*it);
        HalfEdge* he = ed->one_half_edge;
        if(he->twin == nullptr) {
            ed->isborder=true;
        }
    }

    for(auto it = allHalfEdges.begin(); it != allHalfEdges.end(); it++) {
        HalfEdge* he(*it);
        if(he->twin == nullptr) {
            he->isborder = true;
        }
        else {
            he->isborder = false;
        }
    }

    for(auto it = HalfEdge_to_HalfEdgehandle.begin(); it != HalfEdge_to_HalfEdgehandle.end(); it++ ) {
        HalfEdge* he = it->first;
        Vertex* v2 = he->v2();
        std::vector<unsigned int> outgoingHE =v2->get_OutgoingHE();
        for(auto ID: outgoingHE) {
            if (HalfEdgehandle_to_HEdge_map.find(ID) != HalfEdgehandle_to_HEdge_map.end()) {
                HalfEdge* out_HE = HalfEdgehandle_to_HEdge_map[ID];
                if (*he->face == *out_HE->face) {
                    he->next = out_HE;
                    break;
                }
            }
        }
    }


    for(auto it = HalfEdge_to_HalfEdgehandle.begin(); it != HalfEdge_to_HalfEdgehandle.end(); it++ ) {
        HalfEdge* he = it->first;
        Vertex* v1 = he->v1();
        std::vector<unsigned int> incomingHE =v1->get_IncomingHE();
        for(auto ID: incomingHE) {
            if (HalfEdgehandle_to_HEdge_map.find(ID) != HalfEdgehandle_to_HEdge_map.end()) {
                HalfEdge* out_HE = HalfEdgehandle_to_HEdge_map[ID];
                if (*he->face == *out_HE->face) {
                    he->prev = out_HE;
                    break;
                }
            }
        }
    }

}


void Mesh::printdatastructinfo() {

    std::cout << "Size of allVertex " << allVertex.size() << std::endl;
    std::cout << "Size of allFaces " << allFaces.size() << std::endl;
    std::cout << "Size of allEdges " << allEdges.size() << std::endl;
    std::cout << "Size of allHalfEdges " << allHalfEdges.size() << std::endl;

}