#include "Polygons.h"
#include "Mesh3D.h"
//#include "Edge3D.h"
//#include "Face3D.h"
namespace TetElement
{

    Tetrahedron::Tetrahedron(Point_3 **TetPoints, size_t *ids, Mesh3D *pmesh) : Element(pmesh)
    {
        for (int i = 0; i < 4; i++)
        {
            coordinates[i]      = TetPoints[i];
            coordinateIDS[i]    = ids[i];
        }
        parentMesh->registerPoint_3(this);
        establishEdgeConnectivity();
        establishFaceConnectivity();
    }

    Tetrahedron::Tetrahedron(Point_3 *P1, Point_3 *P2, Point_3 *P3, Point_3 *P4, size_t *ids, Mesh3D *pmesh) : Element(pmesh)
    {
        coordinates[0] = P1;
        coordinates[1] = P2;
        coordinates[2] = P3;
        coordinates[3] = P4;
        for (int i = 0; i < 4; i++)
        {
            coordinateIDS[i]    = ids[i];
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

    void Tetrahedron::setPolygonId(size_t id)
    {
        tetID = id;
    }

    void Tetrahedron::setPolygonStatus(bool flag)
    {
        tetStatus = flag;
    }

    size_t Tetrahedron::getPolygonId() const
    {
        return tetID;
    }

    bool Tetrahedron::getPolygonStatus() const
    {
        return tetStatus;
    }

    std::string Tetrahedron::getType() const
    {
        return "Tetrahedron";
    }

    unsigned int Tetrahedron::getCornerSize() const
    {
        unsigned int sz_ = 4;
        return sz_;
    }

    void Tetrahedron::establishEdgeConnectivity()
    {
        unsigned int arr1[] = {1, 2, 0, 3, 3, 3};
        unsigned int arr2[] = {2, 0, 1, 1, 2, 0};
        for (unsigned int i = 0; i < 6; i++)
        {
            Point_3 *p1 = getCorner(arr1[i]);
            Point_3 *p2 = getCorner(arr2[i]);
            Edge_3 obj(p1, p2);
            parentMesh->registerEdge(obj,this);
        }
    }

    void Tetrahedron::establishFaceConnectivity()
    {

        unsigned int idx1[] = {1,2,0};
        unsigned int idx2[] = {2,0,1};
        Face_3* obj = new Face_3 (getCorner(0),getCorner(1),getCorner(2),this);
        parentMesh->registerFace(obj,this);
        for(int i=0; i<3; i++)
        {
            Face_3* Fobj = new Face_3 (getCorner(3),getCorner(idx1[i]),getCorner(idx2[i]),this);
            parentMesh->registerFace(Fobj,this);
        }
    }
}