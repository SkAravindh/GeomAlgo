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