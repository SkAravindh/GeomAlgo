#include "../Tet/IOtet.h"
#include "../Tet/Mesh3D.h"
#include "../Tet/Point3D.cpp"

//took from geeksforgeeks. modified with template
template<typename T>
std::vector<T> removeDupWord(std::string str, std::string vari)
{

    // Used to split string around spaces.
    std::vector<T> ids;
    std::istringstream ss(str);
    std::string word; // for storing each word
    // Traverse through all words
    // while loop till we get
    // strings to store in string word
    int wcount=0;
    while (ss >> word)
    {
        if(word == vari && wcount !=1)
        {
            wcount = wcount +1;
            continue;
        }
        std::stringstream cvt(word);
        T id = 0;
        cvt >> id;
        ids.push_back(id);
        // print the read word
    }
    return ids;
}

TMeshptr ReadVtk(const std::string &filename)
{
    TMeshptr tmesh (new Mesh3D(filename));

    std::fstream myfile;
    myfile.open(filename, std::ios::in);
    if(!myfile.is_open())
    {
        throw "unable to open file ";
    }
    std::string line;
    bool pointFlag     = false;
    bool cellFlag      = false;
    bool celltypeFlag  = false;
    std::vector<int> quadtetType;
    while (std::getline(myfile,line))
    {
        std::string points_str   = line.substr(0,6);
        std::string cell_str     = line.substr(0,5);
        std::string celltype_str = line.substr(0,10);

        if(points_str == "POINTS")
        {
            pointFlag = true;
            continue;
        }
        else if (cell_str == "CELLS")
        {
            pointFlag = false;
            cellFlag  = true;
            continue;
        }
        else if(celltype_str == "CELL_TYPES")
        {
            cellFlag     = false;
            celltypeFlag = true;
            continue;
        }

        if (pointFlag)
        {
            std::vector<double> pts = removeDupWord<double>(line, " ");
            try
            {
                tmesh->addPointsFromVtk(pts);
            }
            catch (const std::string & msg )
            {
                std::cout << msg << std::endl;
            }
        }
        else if (cellFlag)
        {
            if (line[0] == '6')
            {
                std::vector<int> IDS  = removeDupWord<int>(line, "6");
                try
                {
                    tmesh->addPrismFromVtk(IDS);
                }
                catch (const std::string & msg )
                {
                    std::cout << msg << std::endl;
                }
            }
            else if (line[0] == '4')
            {
                std::vector<int> IDS  = removeDupWord<int>(line, "4");
                try
                {
                    tmesh->addTetandQuadFromVtk(IDS);
                }
                catch (const std::string & msg )
                {
                    std::cout << msg << std::endl;
                }
            }
        }
        else  if (celltypeFlag)
        {
           std::vector<int> polygonTP = removeDupWord<int>(line, "6");
            if( polygonTP[0] == 13)
            {
            }
            else if (polygonTP[0] == 10)
            {
                quadtetType.push_back(10);
            }
            else if (polygonTP[0] == 9)
            {
                quadtetType.push_back(9);
            }
        }
    }
    tmesh->splitQuadTet(quadtetType);
    myfile.close();
    tmesh->printContainersInfo();
    return tmesh;
}

void writeTetvtk(const std::string &filename, const std::vector<Point_3*> &vAllVertices, const std::vector<TetElement::Tetrahedron*> &vAllTetrahedron)
{
    std::ofstream vtk_file;
    vtk_file.open(filename+".vtk");
    vtk_file << "# vtk DataFile Version 2.0" << std::endl;
    vtk_file << "volume_Data" << std::endl;
    vtk_file << "ASCII" << std::endl;
    vtk_file << "DATASET UNSTRUCTURED_GRID" << std::endl;

    vtk_file << "POINTS " << vAllVertices.size() << " Float64" << std::endl;
    for(int i=0; i<vAllVertices.size(); i++)
    {
        Point_3 obj = *vAllVertices.at(i);
        vtk_file << obj[0] << " " << obj[1] << " " << obj[2] << std::endl;
    }
    vtk_file << "CELLS " << vAllTetrahedron.size() << " " <<   5 * vAllTetrahedron.size() << std::endl;
    for(TetElement::Tetrahedron* t_element : vAllTetrahedron) {
        vtk_file<< "4 " ;
        for(int i=0; i<4; i++) {
            vtk_file << t_element->getCornerId(i)<< " ";
        }
        vtk_file << std::endl;
    }
    vtk_file << std::endl;

    vtk_file << std::endl;
    vtk_file << "CELL_TYPES " <<vAllTetrahedron.size()<< std::endl;
    for(int i=0; i<vAllTetrahedron.size(); i++) {
        vtk_file << "10 " <<std::endl;
    }
}