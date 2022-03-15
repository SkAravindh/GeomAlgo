#ifndef POLYGON_H
#define POLYGON_H

namespace TriElement
{
    class Triangle
    {
    public:
        Triangle();
        explicit Triangle(Point_3* p1, Point_3* p2, Point_3* p3, Mesh3D* mesh);
        explicit Triangle( Point_3* TetPoints[3], Mesh3D* mesh);
        ~Triangle() noexcept = default;
        Point_3* getCorner(unsigned int id) const;
        size_t getCornerId( unsigned int id) const;
        void addTrianglePointsID(size_t PointsId[3]);

    public:
        size_t triID       = -1;
        bool triStatus     = false;
        Mesh3D* mesh_3D         = nullptr;
    private:
        Point_3* coordinates[3];
        size_t coordinateIDS[3];
    };
}


namespace TetElement
{
    class Tetrahedron
    {
    public:
        Tetrahedron();
        explicit Tetrahedron(Point_3* p1, Point_3* p2, Point_3* p3, Point_3* p4, Mesh3D* mesh);
        explicit Tetrahedron(Point_3* TetPoints[4], Mesh3D* mesh);
        ~Tetrahedron() noexcept = default;
        Point_3* getCorner( unsigned int id) const;
        size_t getCornerId( unsigned int id) const;
        void addTetPointsID(size_t PointsId[4]);
        EdgePts getEO(unsigned int i) const;
        friend std::ostream& operator << (std::ostream &out, const TetElement::Tetrahedron & obj);

    public:
        size_t tetID        = -1;
        bool tetStatus      = false;
        Mesh3D* mesh_3D     = nullptr;
    private:
        Point_3* coordinates[4];
        size_t coordinateIDS[4];
    };
}



namespace QuadElement
{
    class Quad
    {
    public:
        Quad();
        explicit Quad(Point_3* p1, Point_3* p2, Point_3* p3, Point_3* p4, Mesh3D* mesh);
        explicit Quad(Point_3* QuadPoints[4], Mesh3D* mesh);
        ~Quad() noexcept = default;
        Point_3* getCorner(unsigned int id) const;
        size_t getCornerId( unsigned int id) const;
        void addQuadPointsID(size_t PointsId[4]);

    public:
        size_t quadID       = -1;
        bool quadStatus     = false;
        Mesh3D* mesh_3D     = nullptr;
    private:
        Point_3* coordinates[4];
        size_t coordinateIDS[4];
    };
}

namespace PrismElement
{
    class Prism
    {
    public:
        Prism();
        explicit Prism(Point_3* p1, Point_3* p2, Point_3* p3, Point_3* p4,Point_3* p5,Point_3* p6, Mesh3D* mesh);
        explicit Prism(Point_3* prismPoints[6], Mesh3D* mesh);
        ~Prism() noexcept = default;
        Point_3* getCorner(unsigned int id) const;
        size_t getCornerId( unsigned int id) const;
        void addPrismPointsId(size_t prismPointsId[6]);

    public:
        size_t prismID      = -1;
        bool prismStatus    = false;
        Mesh3D* mesh_3D     = nullptr;
    private:
        Point_3* coordinates[6];
        size_t coordinateIDS[6];
    };
}
#endif