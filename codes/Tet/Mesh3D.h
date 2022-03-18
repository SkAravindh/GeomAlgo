#ifndef MESH3D_H
#define MESH3D_H
#include "../Tet/Definition.h"
#include "../Tet/Polygon3D.h"
#include "../Tet/Edge3D.h"
#include "../Tet/Point3D.h"
#include "../Tet/Face3D.h"
#include <map>
#include <set>
#include "unordered_map"

class Mesh3D
        {
public:
    Mesh3D();
    explicit Mesh3D(const std::string &filename_);
    ~Mesh3D() noexcept;
    Point_3* createPoint_3(const Point_3 &p);
    TetElement::Tetrahedron* createTetrahedron(Point_3* tetPoints[4], size_t tetIds[4]);
    QuadElement::Quad* createQuad(Point_3* quadPoints[4], size_t quadIds[4]);
    TriElement::Triangle* createNewTriangle(Point_3* triPoints[3], size_t triIds[3]);
    PrismElement::Prism* createPrism(Point_3* prismPoints[6], size_t prismIds[6]);
    void createPoint_3(const double x, const double y, const double z);
    void addPointsFromVtk(const std::vector<double> &vPoints);
    void addTetandQuadFromVtk(const std::vector<int> &vPoints);
    void addTriangleFromVtk(const std::vector<int> & vIds);
    void addTetAndQuad( const std::vector<int> & vIds , const std::string &arg);
    void addPrismFromVtk(const std::vector<int> & vIds);
    void splitQuadTet(const std::vector<int> & vIds);
    void registerTetrahedron(TetElement::Tetrahedron* tet);
   std::vector<TetElement::Tetrahedron*> getSharedTetrahedron(const EdgePts &ed);
   void getalltetrahedron(  );
    void printContainersInfo();
    void writeTet(const std::string &filename_);
   void registerFace(Face3 * obj);
    void getborder();

private:
    const std::string filename;
    std::vector<Point_3*> vAllVertices;
    std::map<Point_3, Point_3*> mPoint3toPoint3;
    std::map<Point_3*, size_t,ComparePoint3D<double,3>> mPoint3toID;
    std::vector<std::vector<int>> quadtetids;

    std::vector<TetElement::Tetrahedron*> vAllTetrahedron;
    std::vector<QuadElement::Quad*> vAllQuad;
    std::vector<TriElement::Triangle*> vAllTriangle;
    std::vector<PrismElement::Prism*> vAllPrism;

    std::vector<Face3*> allFace;
    //Tetrahedron Data Structure
    std::multimap<Point_3*, TetElement::Tetrahedron* , ComparePoint3D<double,3>> mmPointtoTetrahedron;
   std::unordered_multimap<EdgePts,TetElement::Tetrahedron*,HashIndex<Point_3*,2>> unorderedmapETOE;
   std::map<Face3,Face3*> mFtoF;
   // std::map<EdgePts,TetElement::Tetrahedron*> mmEdgetoTetrahedronl;
};

#endif