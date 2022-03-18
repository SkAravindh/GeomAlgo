#ifndef POLYGONS_H
#define POLYGONS_H
#include "Element.h"

namespace TetElement
{
    class Tetrahedron : public Element
    {
    public:
        explicit Tetrahedron(Point_3* P1, Point_3* P2, Point_3* P3, Point_3* P4, size_t ids[4], Mesh3D* pmesh);
        explicit Tetrahedron(Point_3* TetPoints[4],size_t ids[4], Mesh3D* pmesh);
        ~Tetrahedron() noexcept override = default;
        Point_3* getCorner( unsigned int id) const override;
        size_t getCornerId( unsigned int id) const override;
        void setPolygonId(size_t id) override;
        void setPolygonStatus(bool flag) override;
        size_t getPolygonId() const override;
        bool getPolygonStatus() const override;
        std::string getType() const override;
        unsigned int getCornerSize() const override;
        void establishEdgeConnectivity();
        void establishFaceConnectivity();

    public:
        size_t tetID        = -1;
        bool tetStatus      = false;
        Mesh3D* mesh_3D     = nullptr;
    private:
        Point_3* coordinates[4];
        size_t coordinateIDS[4];
    };
}

#endif