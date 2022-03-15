#include "../Tet/Mesh3D.h"

unsigned int tetIndexBy_one(unsigned int i) {

    if( i < 0 || i > 5) {
        std::cout<< "Index is out of range: " << i << std::endl;
        throw " Index is out of range ";
    }
    unsigned int arr[] = {1,2,0,1,2,0};
    return arr[i];
}

unsigned int tetIndexBy_two(unsigned int i) {

    if( i < 0 || i > 5) {
        std::cout<< "Index is out of range: " << i << std::endl;
        throw " Index is out of range ";
    }
    unsigned int arr[] = {2,0,1,3,3,3};
    return arr[i];
}


namespace TriElement
{
    Triangle::Triangle()
    {
        for (unsigned int i = 0; i < 3; i++)
        {
            coordinates[i]   = nullptr;
            coordinateIDS[i] = -1;
        }
    }

    Triangle::Triangle(Point_3* p1, Point_3* p2, Point_3* p3, Mesh3D* mesh) : mesh_3D(mesh)
    {
        coordinates[0] = p1;
        coordinates[1] = p2;
        coordinates[2] = p3;
    }

    Triangle::Triangle( Point_3* TetPoints[3], Mesh3D* mesh): mesh_3D(mesh)
    {
        for (int i = 0; i < 3; i++)
        {
            coordinates[i] = TetPoints[i];
        }
    }

    Point_3* Triangle::getCorner( unsigned int id) const
    {
        return coordinates[id];
    }

    size_t Triangle::getCornerId( unsigned int id) const
    {
        return coordinateIDS[id];
    }

    void Triangle::addTrianglePointsID(size_t PointsId[3])
    {
        for (int i = 0; i < 3; i++)
        {
            coordinateIDS[i] = PointsId[i];
        }
    }

};

namespace TetElement
{
    Tetrahedron::Tetrahedron()
    {
        for (unsigned int i = 0; i < 4; i++)
        {
            coordinates[i]   = nullptr;
            coordinateIDS[i] = -1;
        }
    }

    Tetrahedron::Tetrahedron(Point_3* p1, Point_3* p2, Point_3* p3, Point_3* p4, Mesh3D* mesh) : mesh_3D(mesh)
    {
        coordinates[0] = p1;
        coordinates[1] = p2;
        coordinates[2] = p3;
        coordinates[3] = p4;
    }

    Tetrahedron::Tetrahedron(Point_3* TetPoints[4], Mesh3D* mesh) : mesh_3D(mesh)
    {
        for (int i = 0; i < 4; i++)
        {
            coordinates[i] = TetPoints[i];
        }
        mesh_3D->registerTetrahedron(this);
    }

    void Tetrahedron::addTetPointsID(size_t PointsId[4])
    {
        for (int i = 0; i < 4; i++)
        {
            coordinateIDS[i] = PointsId[i];
        }
    }

    Point_3* Tetrahedron::getCorner( unsigned int id) const
    {
        return coordinates[id];
    }

    size_t Tetrahedron::getCornerId( unsigned int id) const
    {
        return coordinateIDS[id];
    }

    EdgePts Tetrahedron::getEO(unsigned int i) const
    {
        Point_3* p0 = this->getCorner(tetIndexBy_one(i));
        Point_3* p1 = this->getCorner(tetIndexBy_two(i));
        EdgePts Ed_obj(p0,p1);
        return Ed_obj;
    }
};
std::ostream& operator << (std::ostream &out, const TetElement::Tetrahedron & obj)
{
    out << *obj.getCorner(0) << " " << *obj.getCorner(1) << " " << *obj.getCorner(2) << " " << *obj.getCorner(3);
    return out;
}
namespace QuadElement
{
    Quad::Quad()
    {
        for (unsigned int i = 0; i < 4; i++) {
            coordinates[i]   = nullptr;
            coordinateIDS[i] = -1;
        }
    }

    Quad::Quad(Point_3* p1, Point_3* p2, Point_3* p3, Point_3* p4, Mesh3D* mesh) : mesh_3D(mesh)
    {
        coordinates[0] = p1;
        coordinates[1] = p2;
        coordinates[2] = p3;
        coordinates[3] = p4;
    }

    Quad::Quad(Point_3* QuadPoints[4], Mesh3D* mesh) : mesh_3D(mesh)
    {
        for(int i=0; i<4; i++)
        {
            coordinates[i] = QuadPoints[i];
        }
    }

    void Quad::addQuadPointsID(size_t PointsId[4])
    {
        for (int i = 0; i < 4; i++)
        {
            coordinateIDS[i] = PointsId[i];
        }
    }

    Point_3* Quad::getCorner(unsigned int id) const
    {
        return coordinates[id];
    }

    size_t Quad::getCornerId(unsigned int id) const
    {
        return coordinateIDS[id];
    }
};

namespace PrismElement
{
    Prism::Prism()
    {
        for (unsigned int i = 0; i < 6; i++)
        {
            coordinates[i]      = nullptr;
            coordinateIDS[i]    = -1;
        }
    }

    Prism::Prism(Point_3* p1, Point_3* p2, Point_3* p3, Point_3* p4,Point_3* p5,Point_3* p6, Mesh3D* mesh) : mesh_3D(mesh)
    {
        coordinates[0] = p1;
        coordinates[1] = p2;
        coordinates[2] = p3;
        coordinates[3] = p4;
        coordinates[4] = p5;
        coordinates[5] = p6;
    }

    Prism::Prism(Point_3* prismPoints[6], Mesh3D* mesh) : mesh_3D(mesh)
    {
        for (int i = 0; i < 6; i++)
        {
            coordinates[i]     = prismPoints[i];
        }
    }

    Point_3* Prism::getCorner(unsigned int id) const
    {
        return coordinates[id];
    }

    size_t Prism::getCornerId( unsigned int id) const
    {
        return coordinateIDS[id];
    }

    void Prism::addPrismPointsId(size_t prismPointsId[6])
    {
        for (int i = 0; i < 6; i++)
        {
            coordinateIDS[i]     = prismPointsId[i];
        }
    }
};