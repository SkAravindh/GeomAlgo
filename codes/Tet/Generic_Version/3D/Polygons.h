#ifndef POLYGONS_H
#define POLYGONS_H
#include "Element.h"

class Mesh3D;
typedef Face3D<Point_3* , Element*> Face_3;

namespace TetElement
{
    class Tetrahedron : public Element
    {
    public:
        Tetrahedron() = default;
        explicit Tetrahedron(Point_3* P1, Point_3* P2, Point_3* P3, Point_3* P4, size_t ids[4], Mesh3D* pmesh);
        explicit Tetrahedron(Point_3* TetPoints[4],size_t ids[4], Mesh3D* pmesh);
        ~Tetrahedron() noexcept override = default;
        Point_3* getCorner( unsigned int id) const override;
        size_t getCornerId( unsigned int id) const override;
        size_t getPolygonId() const override;
        std::string getType() const override;
        unsigned int getCornerSize() const override;
        bool getPolygonStatus() const override;
        bool _equal(Element const & rhs) const override;
        bool _notequal(Element const & rhs) const override;
        void setPolygonId(size_t id) override;
        void setPolygonStatus(bool flag) override;
        void establishEdgeConnectivity();
        void establishFaceConnectivity();
        void establishFacetoEdgeConnectivity(Face_3* obj);
        friend std::ostream& operator << (std::ostream &out,   TetElement::Tetrahedron &rhs)
        {
            out<<"p0  "<<rhs.getCorner(0)<<std::endl;
            out<<"p1  "<<rhs.getCorner(1)<<std::endl;
            out<<"p2  "<<rhs.getCorner(2)<<std::endl;
            out<<"p3  "<<rhs.getCorner(3)<<std::endl;
            return out;
        }
    private:
        size_t tetID        = -1;
        bool tetStatus      = false;
        Mesh3D* mesh_3D     = nullptr;
    private:
        Point_3* coordinates[4];
        size_t coordinateIDS[4];
    };
}


namespace TriElement
{
    class Triangle : public Element
    {
    public:
        Triangle() = default;
        explicit Triangle(Point_3* P1, Point_3* P2, Point_3* P3,size_t ids[3], Mesh3D* pmesh);
        explicit Triangle(Point_3* TriPoints[3],size_t ids[3], Mesh3D* pmesh);
        ~Triangle() noexcept override = default;
        Point_3* getCorner( unsigned int id) const override;
        size_t getCornerId( unsigned int id) const override;
        size_t getPolygonId() const override;
        std::string getType() const override;
        unsigned int getCornerSize() const override;
        bool getPolygonStatus() const override;
        bool _equal(Element const & rhs) const override;
        bool _notequal(Element const & rhs) const override;
        void setPolygonId(size_t id) override;
        void setPolygonStatus(bool flag) override;

    private:
        size_t triID        = -1;
        bool triStatus      = false;
        Mesh3D* mesh_3D     = nullptr;
    private:
        Point_3* coordinates[3];
        size_t coordinateIDS[3];
    };
}

#endif