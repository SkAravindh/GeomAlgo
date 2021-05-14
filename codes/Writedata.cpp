#include "Writedata.h"

void writePoints(const std::string& filename, std::vector<Point*> &bp) {

    std::ofstream vtk_file;
    vtk_file.open(filename);
    vtk_file << "# vtk DataFile Version 2.0" << std::endl;
    vtk_file << "Heat Clusters" << std::endl;
    vtk_file << "ASCII" << std::endl;
    vtk_file << "DATASET UNSTRUCTURED_GRID" << std::endl;
    vtk_file << "POINTS " << bp.size() << " float" << std::endl;
    for (unsigned int i = 0; i < bp.size(); ++i) {
        vtk_file << bp[i]->x() << " " << bp[i]->y() << " "
                 << bp[i]->z() << std::endl;
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

void writeSTL(const std::string& filename, std::vector<Triangle* > &patch) {

    std::ofstream file;
    file.open(filename,std::ios_base::app);
    file <<"solid " << std::endl;
    for(int i=0; i<patch.size(); i++){
        //file <<" facet normal "<<patch.at(i)->getNormalVector().x()<<std::scientific<<" "<<patch.at(i)->getNormalVector().y()<<std::scientific<<" "<<patch.at(i)->getNormalVector().z()<<std::scientific<<std::endl;
        file <<" facet normal "<<patch.at(i)->getNormalVector().x()<<" "<<patch.at(i)->getNormalVector().y()<<" "<<patch.at(i)->getNormalVector().z()<<std::endl;
        file <<"  outer loop "<< std::endl;
        file <<"    vertex "<<patch[i]->getCorners(0)->x()<<" "<<patch[i]->getCorners(0)->y()<<" "<<patch[i]->getCorners(0)->z()<<std::endl;
        file <<"    vertex "<<patch[i]->getCorners(1)->x()<<" "<<patch[i]->getCorners(1)->y()<<" "<<patch[i]->getCorners(1)->z()<<std::endl;
        file <<"    vertex "<<patch[i]->getCorners(2)->x()<<" "<<patch[i]->getCorners(2)->y()<<" "<<patch[i]->getCorners(2)->z()<<std::endl;
        file <<"  endloop "<<std::endl;
        file <<" endfacet "<<std::endl;
    }
    file <<"endsolid "<<std::endl;

}

void writeVTK(const std::string& filename, std::shared_ptr<Mesh> &mesh) {

    storevertex SV;
    std::ofstream vtk_file;
    vtk_file.open(filename);
    vtk_file << "# vtk DataFile Version 2.0" << std::endl;
    vtk_file << "Mesh_Data" << std::endl;
    vtk_file << "ASCII" << std::endl;
    vtk_file << "DATASET POLYDATA" <<std::endl;
    std::vector<Point*> allvertex;
    std::vector<Triangle*> alltriangles;
    mesh->getVertices(allvertex);
    mesh->getTriangles(alltriangles);
    int count = 0;
    vtk_file << "POINTS " << allvertex.size() << " Float64" << std::endl;
    for(auto it = allvertex.begin(); it!= allvertex.end(); it++) {
        SV.add(*it,count);
        ++count;
        vtk_file << (*it)->x() << " " << (*it)->y() << " " << (*it)->z() << std::endl;
    }
    vtk_file << "POLYGONS " <<  " " << alltriangles.size() << " " << alltriangles.size() * 4 << std::endl;
    for(auto it = alltriangles.begin(); it != alltriangles.end(); it++) {

        Point* v0 = (*it)->getCorners(0);
        Point* v1 = (*it)->getCorners(1);
        Point* v2 = (*it)->getCorners(2);
        vtk_file << "3 " << " " << SV[v0]<< " " << SV[v1] << " " << SV[v2] << std::endl;
    }
    vtk_file << "CELL_DATA " << alltriangles.size() << std::endl;
    vtk_file << "NORMALS cell_normals Float64" << std::endl;
    for(auto it = alltriangles.begin(); it != alltriangles.end(); it++) {

        Vector3 nnv = (*it)->getNormalVector();
        vtk_file << nnv[0] << " " << nnv[1] << " " << nnv[2] << std::endl;
    }
    vtk_file.close();
}