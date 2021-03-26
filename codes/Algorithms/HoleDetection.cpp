#include "HoleDetection.h"

HoleDetection::HoleDetection(const std::string &filename) {
    OpenMesh::IO::read_mesh(mesh, filename);
}

double HoleDetection::computeGaussianCurvature(const VertexHandle &Vertex) {

    if (!Vertex.is_valid()) {
        return 0.0;
    }
    point Input_Vertex = mesh.point(Vertex);
    VertexOHalfedgeIter voh_it = mesh.voh_iter(Vertex);

    if (!voh_it.is_valid()) {
        return 0.0;
    }
    double theta_sum=0;
    for( voh_it = mesh.voh_iter(Vertex); voh_it; ++voh_it) {

        VertexHandle CurrentOutGoingHEVertex = voh_it->to();
        HalfedgeHandle NextOutGoingHE = voh_it->next();

        VertexHandle n_voh_it = mesh.to_vertex_handle(NextOutGoingHE);
        point CurrentPoint = mesh.point(CurrentOutGoingHEVertex);
        point NextPoint = mesh.point(n_voh_it);

        point Edge_1 = CurrentPoint - Input_Vertex;
        point Edge_2 = NextPoint - Input_Vertex;
        double theta = getAngleBtwVectors(Edge_1, Edge_2);
        theta_sum = theta_sum + theta;
    }

    //Calculation of Area of one ring neighbourhood
    VertexFaceIter vf_iter = mesh.vf_iter(Vertex);
    if(!vf_iter.is_valid()) {
        return 0.0;
    }

    double area_sum = 0;
    for(vf_iter = mesh.vf_iter(Vertex); vf_iter; ++vf_iter) {
        MyMesh::ConstFaceVertexRange CurrentFace = vf_iter->vertices();
        std::vector<VertexHandle> vertex_ids;
        for(auto f_id = CurrentFace.begin(); f_id != CurrentFace.end(); f_id++) {
            vertex_ids.push_back(*f_id);
        }
        double Area =  calculateArea(vertex_ids);
        area_sum = area_sum+Area;
    }

    double k = (Gaussian_Curvature - theta_sum)/area_sum;
    //std::cout<<"k is " <<k<<std::endl;
    return k;
}

void HoleDetection::computeHoles() {

    std::vector<EdgeHandle> possible_edges;
    for (MyMesh::EdgeIter edge_iter = mesh.edges_begin(); edge_iter != mesh.edges_end(); edge_iter++) {

        if (edge_iter->is_boundary()) continue;
        HalfedgeHandle Current_HE = mesh.halfedge_handle(*edge_iter, 0);
        HalfedgeHandle Opposite_HE = mesh.opposite_halfedge_handle(Current_HE);
        point Current_Face_Normal = mesh.calc_face_normal(mesh.face_handle(Current_HE));
        point Opposite_Face_Normal = mesh.calc_face_normal(mesh.face_handle(Opposite_HE));
        double degree = (getAngleBtwVectors(Current_Face_Normal, Opposite_Face_Normal) * 180) / PI;
        //  std::cout<<"degree " <<degree << std::endl;
        if (degree > 30 && degree < 130) {
            possible_edges.push_back(*edge_iter);
        }
    }
    
    std::map<VertexHandle, bool> vertex_to_bool;
    std::vector<EdgeHandle> possible_edges_1;
    
    for(MyMesh::VertexIter it = mesh.vertices_begin(); it != mesh.vertices_end(); it++) {
        double re = computeGaussianCurvature(*it);
        if(re <0) {
            vertex_to_bool[*it] = true;
        }
        else {
            vertex_to_bool[*it] = false;
        }
    }
    
    for (auto iter = possible_edges.begin(); iter != possible_edges.end(); iter++) {
        
        VertexHandle to    = mesh.to_vertex_handle(mesh.halfedge_handle(*iter,0));
        VertexHandle from  = mesh.from_vertex_handle(mesh.halfedge_handle(*iter,0));
        bool v1 = vertex_to_bool[to];
        bool v2 = vertex_to_bool[from];
        if(v1&&v2) {
            possible_edges_1.push_back(*iter);
        }
    }

    removeDuplicates(possible_edges_1);

    std::vector<point> allpoints;
    //  createLoop(possible_edges_1);
    for(auto ele: possible_edges_1) {
        point p1 = mesh.point(mesh.to_vertex_handle(mesh.halfedge_handle(ele,0)));
        point p2 = mesh.point(mesh.from_vertex_handle(mesh.halfedge_handle(ele,0)));
        allpoints.push_back(p1);
        allpoints.push_back(p2);
    }
    writePoints("feature_vertices.vtk",allpoints);
}

void HoleDetection::removeDuplicates(std::vector<EdgeHandle> &possible_edges_1) {

    bool stop = true;
    while(stop) {
        std::multimap<VertexHandle, EdgeHandle> vertex_to_Edge;
        std::set<VertexHandle> allvertex;
        for (auto it = possible_edges_1.begin(); it != possible_edges_1.end(); it++) {
            VertexHandle to = mesh.to_vertex_handle(mesh.halfedge_handle(*it, 0));
            VertexHandle from = mesh.from_vertex_handle(mesh.halfedge_handle(*it, 0));
            allvertex.insert(to);
            allvertex.insert(from);
            vertex_to_Edge.insert(std::make_pair(to, *it));
            vertex_to_Edge.insert(std::make_pair(from, *it));
        }

        int stopcriteria = 0;
        typedef std::multimap<VertexHandle, EdgeHandle>::iterator MMIT;
        std::set<VertexHandle> vertexwith_oneedge;
        for (auto it = allvertex.begin(); it != allvertex.end(); it++) {
            std::pair<MMIT, MMIT> sharingedges = vertex_to_Edge.equal_range(*it);
            int size = std::distance(sharingedges.first, sharingedges.second);
            if (size == 1) {
                vertexwith_oneedge.insert(*it);
                ++stopcriteria;
            }
        }
        if(stopcriteria == 0) {
            break;
        }

        std::set<VertexHandle> dummy_vertex(vertexwith_oneedge.begin(), vertexwith_oneedge.end());
        while (!dummy_vertex.empty()) {

            VertexHandle Initial_v1 = *dummy_vertex.begin();
            EdgeHandle Initial_edge;
            VertexHandle Initial_v2;
            dummy_vertex.erase(Initial_v1);
            std::pair<MMIT, MMIT> edge = vertex_to_Edge.equal_range(Initial_v1);
            int dis = std::distance(edge.first, edge.second);
            if (dis != 1) continue;
            for (auto iter = edge.first; iter != edge.second; iter++) {
                Initial_edge = iter->second;
            }
            getNextVertexHandle(mesh, Initial_edge, Initial_v1, Initial_v2);
            duplicateedges.insert(Initial_edge);
            VertexHandle currentvertex = Initial_v2;
            EdgeHandle currentedge = Initial_edge;
            VertexHandle nextvertex;
            EdgeHandle nextedge;
            while (getNextDuplicateEdge(currentvertex, currentedge, nextvertex, nextedge, vertex_to_Edge)) {
                dummy_vertex.erase(nextvertex);
                currentvertex = nextvertex;
                currentedge = nextedge;
                duplicateedges.insert(nextedge);
                if (nextedge == Initial_edge) {
                    break;
                }
            }
        }
        for (auto ele : duplicateedges) {
            std::vector<EdgeHandle>::iterator position = std::find(possible_edges_1.begin(), possible_edges_1.end(),
                                                                   ele);
            if (position != possible_edges_1.end()) {
                possible_edges_1.erase(position);
            }
        }
        duplicateedges.clear();
    }

}

bool HoleDetection::getNextDuplicateEdge(VertexHandle &curent, EdgeHandle &Initial, VertexHandle &next_vertex, EdgeHandle &next_edge,std::multimap<VertexHandle,EdgeHandle> &vertex_to_Edge) {

    typedef std::multimap<VertexHandle, EdgeHandle>::iterator it;
    std::pair<it, it> edges = vertex_to_Edge.equal_range(curent);
    int dis = std::distance(edges.first, edges.second);
    if (dis == 1) {
        for (auto iter = edges.first; iter != edges.second; iter++) {
            next_edge = iter->second;
            getNextVertexHandle(mesh, next_edge, curent, next_vertex);
        }
        return true;
    } else if (dis == 2) {
        for (auto iter = edges.first; iter != edges.second; iter++) {
            if (iter->second == Initial) continue;
            next_edge = iter->second;
            getNextVertexHandle(mesh, next_edge, curent, next_vertex);
        }
        return true;
    }
    return false;
}
/*
void HoleDetection::createLoop(std::set<HalfedgeHandle> &possible_edges_1) {

    std::cout<<"looping has started " << std::endl;
    std::multimap<MyMesh::VertexHandle,MyMesh::HalfedgeHandle> vertex_to_edge;
    std::set<VertexHandle> allvertex;
    for(auto ele : possible_edges_1) {
        MyMesh::VertexHandle to = mesh.to_vertex_handle(ele);
        MyMesh::VertexHandle from = mesh.from_vertex_handle(ele);
        allvertex.insert(to);
        allvertex.insert(from);
        vertex_to_edge.insert(std::make_pair(to,ele));
        vertex_to_edge.insert(std::make_pair(from,ele));
    }
    std::set<MyMesh::HalfedgeHandle> he_dummy(possible_edges_1.begin(),possible_edges_1.end());
    int count=0;
    while (!he_dummy.empty()) {
        ++count;
        MyMesh::HalfedgeHandle intihe = *he_dummy.begin();
        Looping* obj = new Looping;
        loops.push_back(obj);
        obj->addEdge(intihe);
        he_dummy.erase(intihe);
        MyMesh::HalfedgeHandle currhe = intihe;
        MyMesh::HalfedgeHandle next_he;
        while(getNextHE(currhe,next_he,vertex_to_edge)) {

            obj->addEdge(next_he);
            currhe=next_he;
            he_dummy.erase(next_he);
            if(obj->isclosed(mesh)) {
                break;
            }
        }
        //std::cout<< obj->he_vector.size() << std::endl;
    }
    std::cout<<"count " <<count << std::endl;
}

bool HoleDetection::getNextHE(HalfedgeHandle &curr_he, HalfedgeHandle &next_he, std::multimap<VertexHandle,HalfedgeHandle> &vertex_to_edge) {

    typedef std::multimap<MyMesh::VertexHandle,MyMesh::HalfedgeHandle>::iterator it;
    MyMesh::VertexHandle from = mesh.from_vertex_handle(curr_he);
    std::pair<it,it> edges = vertex_to_edge.equal_range(from);
    int dis = std::distance(edges.first,edges.second);
    if(dis != 2 ) {
        return false;
    }
    for(it iter = edges.first; iter != edges.second; iter++) {
        if(curr_he == iter->second) continue;
        next_he = iter->second;
        return true;
    }
    return false;
}
*/
double HoleDetection::calculateArea(std::vector<VertexHandle> &vertex_ids) {

    point P0 = mesh.point(vertex_ids[0]);
    point P1 = mesh.point(vertex_ids[1]);
    point P2 = mesh.point(vertex_ids[2]);
    double D1 = getDistance(P0,P1);
    double D2 = getDistance(P1,P2);
    double D3 = getDistance(P2,P1);
    double s= (D1+D2+D3)/2;
    double AR = (D1*D2*D3)/(8*(s-D1)*(s-D2)*(s-D3));
    return abs(AR);
}

void getNextVertexHandle(const MyMesh &mesh, const EdgeHandle &Input_Edge, const VertexHandle &input,  VertexHandle &out) {

    VertexHandle v1 = mesh.to_vertex_handle( (mesh.halfedge_handle(Input_Edge,0) ));
    VertexHandle v2 = mesh.from_vertex_handle( (mesh.halfedge_handle(Input_Edge,0) ));
    if(input == v1) {
        out = v2;
    }
    else {
        out = v1;
    }
}

void Looping::addEdge(HalfedgeHandle &he) {
    he_vector.push_back(he);
}

bool Looping::isclosed(const MyMesh &mesh) {

    MyMesh::HalfedgeHandle start = *he_vector.begin();
    MyMesh::HalfedgeHandle end = he_vector.back();
    MyMesh::VertexHandle fi = mesh.from_vertex_handle(start);
    MyMesh::VertexHandle las = mesh.to_vertex_handle(end);
    if(fi==las) {
        return true;
    }
    else {
        return false;
    }
}

double getAngleBtwVectors(const point &p1, const point &p2) {

    double p1_length  = p1.length();
    double p2_length  = p2.length();
    double dotproduct = p1.dot(p2);
    double angle = acos( dotproduct/(p1_length * p2_length) );
    return angle;
}

double getDistance(const point &p1, const point &p2)  {
    return (std::sqrt(std::pow((p2[0]-p1[0]),2) + std::pow((p2[1]-p1[1]),2) + std::pow((p2[2]-p1[2]),2)));
}

void writePoints(std::string filename, std::vector<point> &bp) {

    std::ofstream vtk_file;
    vtk_file.open(filename);
    vtk_file << "# vtk DataFile Version 2.0" << std::endl;
    vtk_file << "Heat Clusters" << std::endl;
    vtk_file << "ASCII" << std::endl;
    vtk_file << "DATASET UNSTRUCTURED_GRID" << std::endl;
    vtk_file << "POINTS " << bp.size() << " float" << std::endl;
    for (unsigned int i = 0; i < bp.size(); ++i) {
        vtk_file << bp[i][0] << " " << bp[i][1] << " "
                 << bp[i][2] << std::endl;
    }
    vtk_file << "CELLS " << bp.size() << " " << bp.size() * 2 << std::endl;
    for (unsigned int i = 0; i < bp.size(); ++i) {
        vtk_file << "1" << " " << i << std::endl;
    }
    vtk_file << "CELL_TYPES " << bp.size() << std::endl;
    for (unsigned int i = 0; i < bp.size(); ++i) {
        vtk_file << "1" << std::endl;
    }
    vtk_file << "POINT_DATA " << bp.size() << std::endl;
    vtk_file << "SCALARS scalars float 1" << std::endl;
    vtk_file << "LOOKUP_TABLE default" << std::endl;
    for (unsigned int i = 0; i < bp.size(); ++i) {
        vtk_file << i << std::endl;
    }
    vtk_file.close();
}
