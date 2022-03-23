#include "TetIO.h"
#include "Mesh3D.h"
#include "Polygons.h"

IOVtk::IOVtk()
{}

void IOVtk::storePoint(const std::vector<double> &vPoints)
{
    if (vPoints.size() != 3)
    {
        throw "Size is not three at storePoint ";
    }
    allpoints.push_back(vPoints);
}

void IOVtk::addTetandQuadids(const std::vector<int> &vPoints)
{
    if(vPoints.size() != 4)
    {
        throw "Size is not Four at addTetandQuadids ";
    }
    quadtetids.push_back(vPoints);
}

void IOVtk::storePrism(const std::vector<int> &vPoints)
{
    if(vPoints.size() != 4)
    {
        throw "Size is not Four at addTetandQuadids ";
    }
    allprismIDS.push_back(vPoints);
}

void IOVtk::splitQuadTet(const std::vector<int> & vIds)
{
    if(vIds.empty())
    {
        return;
    }
    for(int i=0; i<vIds.size(); i++)
    {
        if(vIds.at(i) == 10)
        {
            std::vector<int> tetIds = quadtetids.at(i);
            alltetrahedronIDS.push_back(tetIds);
         //   storeTetandQuad(tetIds, "Tet");
        }
        if(vIds.at(i) == 9)
        {
            std::vector<int> quadIds = quadtetids.at(i);
            allquadIDS.push_back(quadIds);
          //  storeTetandQuad(quadIds, "Quad");
        }
    }
}

TMesh IOVtk::buildMesh()
{

     pmesh = std::make_shared<Mesh3D>( Mesh3D("MeshFromVTK"));
    for(int i=0; i<allpoints.size(); i++)
    {
        std::vector<double> currentPts = allpoints.at(i);
        pmesh->createPoint_3(currentPts[0],currentPts[1],currentPts[2]);
    }

    const std::vector<Point_3*>& vAllVertices  = pmesh->getallVertices();
    std::map<Point_3*, size_t,ComparePoint3D<double,3>> mPoint3toID = pmesh->getmPoint3toID();

    for(int i=0; i<alltetrahedronIDS.size(); i++)
    {
        std::vector<int> currentTet = alltetrahedronIDS.at(i);
        if(currentTet.size() != 4)
        {
            throw "Size is not Four at alltet ";
        }
        Point_3* points[4];
        size_t   pointsID[4];
        for(int j=0; j<4; j++)
        {
            points[j]    = vAllVertices.at(currentTet.at(j));
            pointsID[j]  = mPoint3toID[points[j]];
        }
        pmesh->createTetrahedron(points,pointsID);
    }

    return pmesh;
}

void IOVtk::print()
{
    std::cout << "allpoints " << allpoints.size() << std::endl;
    std::cout << "alltriangleIDS " << alltriangleIDS.size() << std::endl;
    std::cout << "allquadIDS " << allquadIDS.size() << std::endl;
    std::cout << "alltetrahedronIDS " << alltetrahedronIDS.size() << std::endl;
    std::cout << "allprismIDS " << allprismIDS.size() << std::endl;
    std::cout << "allhexIDS " << allhexIDS.size() << std::endl;
    std::cout << "quadtetids " << quadtetids.size() << std::endl;

}

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

Vector3 getNormal(const TriElement::Triangle* t)
{
    Point_3 *p0 = t->getCorner(0);
    Point_3 *p1 = t->getCorner(1);
    Point_3 *p2 = t->getCorner(2);

    Vector3 vertex0 = Vector3(p0->getCorners(0), p0->getCorners(1), p0->getCorners(2));
    Vector3 vertex1 = Vector3(p1->getCorners(0), p1->getCorners(1), p1->getCorners(2));
    Vector3 vertex2 = Vector3(p2->getCorners(0), p2->getCorners(1), p2->getCorners(2));

    Vector3 edge_1 = vertex1 - vertex0;
    Vector3 edge_2 = vertex2 - vertex0;
    Vector3 normal = cross_product(edge_1,edge_2);
    return normal;
}

TMesh ReadVtk(const std::string &filename)
{

    IOVtk vtkmesh = IOVtk();

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
                //tmesh->addPointsFromVtk(pts);
                vtkmesh.storePoint(pts);
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
                  //  tmesh->addPrismFromVtk(IDS);
                    vtkmesh.storePrism(IDS);
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
                  //  tmesh->addTetandQuadFromVtk(IDS);
                    vtkmesh.addTetandQuadids(IDS);
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
    vtkmesh.splitQuadTet(quadtetType);
    myfile.close();
    vtkmesh.print();
    TMesh tmesh = vtkmesh.buildMesh();
    return tmesh;
}

void writevtkPoints(const std::string& filename, std::vector<Point_3 > &bp)
{
    std::ofstream vtk_file;
    vtk_file.open(filename+".vtk");
    vtk_file << "# vtk DataFile Version 2.0" << std::endl;
    vtk_file << "Heat Clusters" << std::endl;
    vtk_file << "ASCII" << std::endl;
    vtk_file << "DATASET UNSTRUCTURED_GRID" << std::endl;
    vtk_file << "POINTS " << bp.size() << " float" << std::endl;
    for (unsigned int i = 0; i < bp.size(); ++i) {
        vtk_file << bp[i].getCorners(0) << " " << bp[i].getCorners(1) << " "
                 << bp[i].getCorners(2) << std::endl;
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

void writeSTL(const std::string& filename, std::vector<TriElement::Triangle*> &patch)
{

    std::ofstream file;
    file.open(filename+".stl",std::ios_base::app);
    file <<"solid " << std::endl;
    for(int i=0; i<patch.size(); i++)
    {
       Vector3 norm = getNormal(patch.at(i));
        file <<" facet normal "<<norm.x()<<std::scientific<<" "<<norm.y()<<std::scientific<<" "<<norm.z()<<std::scientific<<std::endl;
        //file <<" facet normal "<<patch.at(i)->getNormalVector().x()<<" "<<patch.at(i)->getNormalVector().y()<<" "<<patch.at(i)->getNormalVector().z()<<std::endl;
        file <<"  outer loop "<< std::endl;
        file <<"    vertex "<<patch[i]->getCorner(0)->getCorners(0)<<" "<<patch[i]->getCorner(0)->getCorners(1)<<" "<<patch[i]->getCorner(0)->getCorners(2)<<std::endl;
        file <<"    vertex "<<patch[i]->getCorner(1)->getCorners(0)<<" "<<patch[i]->getCorner(1)->getCorners(1)<<" "<<patch[i]->getCorner(1)->getCorners(2)<<std::endl;
        file <<"    vertex "<<patch[i]->getCorner(2)->getCorners(0)<<" "<<patch[i]->getCorner(2)->getCorners(1)<<" "<<patch[i]->getCorner(2)->getCorners(2)<<std::endl;
        file <<"  endloop "<<std::endl;
        file <<" endfacet "<<std::endl;
    }
    file <<"endsolid "<<std::endl;
}

