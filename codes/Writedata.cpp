#include "Writedata.h"

void writePoint(std::vector<EdgeOrder> &oe) {
    std::vector<Point *> point_to_id_map;
    for (auto ele : oe) {
        point_to_id_map.push_back(ele.p0);
        point_to_id_map.push_back(ele.p1);
    }


    std::ofstream vtk_file;
    vtk_file.open("points.vtk");
    vtk_file << "# vtk DataFile Version 2.0" << std::endl;
    vtk_file << "Heat Clusters" << std::endl;
    vtk_file << "ASCII" << std::endl;
    vtk_file << "DATASET UNSTRUCTURED_GRID" << std::endl;
    vtk_file << "POINTS " << point_to_id_map.size() << " float" << std::endl;
    for (unsigned int i = 0; i < point_to_id_map.size(); ++i) {
        vtk_file << point_to_id_map[i]->x() << " " << point_to_id_map[i]->y() << " "
                 << point_to_id_map[i]->z() << std::endl;
    }
    vtk_file << "CELLS " << point_to_id_map.size() << " " << point_to_id_map.size() * 2 << std::endl;
    for (unsigned int i = 0; i < point_to_id_map.size(); ++i) {
        vtk_file << "1" << " " << i << std::endl;
    }
    vtk_file << "CELL_TYPES " << point_to_id_map.size() << std::endl;
    for (unsigned int i = 0; i < point_to_id_map.size(); ++i) {
        vtk_file << "1" << std::endl;
    }
    vtk_file << "POINT_DATA " << point_to_id_map.size() << std::endl;
    vtk_file << "SCALARS scalars float 1" << std::endl;
    vtk_file << "LOOKUP_TABLE default" << std::endl;
    for (unsigned int i = 0; i < point_to_id_map.size(); ++i) {
        vtk_file << i << std::endl;
    }
    vtk_file.close();
}

void writeSTL(std::string filename, std::vector<Triangle* > &patch){
    std::ofstream file;
    file.open(filename,std::ios_base::app);
    file <<"solid " << std::endl;
    for(int i=0; i<patch.size(); i++){
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