#include "../Tet/Mesh3D.h"
#include "../Tet/Point3D.cpp"
#include "../Tet/IOtet.h"

Mesh3D::Mesh3D()
{
}

Mesh3D::Mesh3D(const std::string &filename_) : filename(filename_)
{
}

Mesh3D::~Mesh3D() noexcept
{
    for(Point_3* p : vAllVertices)
    {
        delete p;
    }
    for(TriElement::Triangle* Tri : vAllTriangle)
    {
        delete Tri;
    }
    for(TetElement::Tetrahedron* Tet : vAllTetrahedron)
    {
        delete Tet;
    }
    for(QuadElement::Quad* Qd : vAllQuad)
    {
        delete Qd;
    }
    for(PrismElement::Prism* Psm : vAllPrism)
    {
        delete Psm;
    }
}

void Mesh3D::createPoint_3(const double x, const double y, const double z)
{
    Point_3 p(x,y,z);
    createPoint_3(p);
}

Point_3* Mesh3D::createPoint_3(const Point_3 &p)
{
    Mpoint_3iter it = mPoint3toPoint3.find(p);
    if(it != mPoint3toPoint3.end())
    {
        return it->second;
    }
    else
    {
        size_t size_ = vAllVertices.size();
        Point_3* pt  = new Point_3(p[0],p[1],p[2]);
        pt->setID(size_);
        pt->setAlive(true);
        mPoint3toPoint3.insert({p,pt});
        mPoint3toID.insert({pt,size_});
        vAllVertices.push_back(pt);
        return pt;
    }
}

TriElement::Triangle* Mesh3D::createNewTriangle(Point_3 *triPoints[3], size_t triIds[3])
{
    TriElement::Triangle* triObj    = new TriElement::Triangle(triPoints,this);
    size_t size_                    = vAllTriangle.size();
    triObj->triID                   = size_;
    triObj->triStatus               = true;
    triObj->addTrianglePointsID(triIds);
    vAllTriangle.push_back(triObj);
    return triObj;
}

TetElement::Tetrahedron* Mesh3D::createTetrahedron(Point_3* tetPoints[4], size_t tetIds[4])
{
    TetElement::Tetrahedron* tetObj = new TetElement::Tetrahedron(tetPoints,this);
    size_t size_                    = vAllTetrahedron.size();
    tetObj->tetID                   = size_;
    tetObj->tetStatus               = true;
    tetObj->addTetPointsID(tetIds);
    vAllTetrahedron.push_back(tetObj);
    return tetObj;
}

QuadElement::Quad* Mesh3D::createQuad(Point_3* quadPoints[4], size_t quadIds[4])
{
    QuadElement::Quad* quadObj  = new QuadElement::Quad(quadPoints,this);
    size_t size_                = vAllQuad.size();
    quadObj->quadID             = size_;
    quadObj->quadStatus         = true;
    quadObj->addQuadPointsID(quadIds);
    vAllQuad.push_back(quadObj);
    return quadObj;
}

PrismElement::Prism* Mesh3D::createPrism(Point_3* prismPoints[6],  size_t prismIds[6])
{
    PrismElement::Prism* prismObj       = new PrismElement::Prism(prismPoints,this);
    size_t size_                        = vAllPrism.size();
    prismObj->prismID                   = size_;
    prismObj->prismStatus               = true;
    prismObj->addPrismPointsId(prismIds);
    vAllPrism.push_back(prismObj);
    return prismObj;
}

void Mesh3D::addPointsFromVtk(const std::vector<double> &vPoints)
{
    if(vPoints.size() != 3)
    {
        throw "Size is not three at addPointsFromVtk ";
    }
    createPoint_3(vPoints[0],vPoints[1],vPoints[2]);
}

void Mesh3D::addTetandQuadFromVtk(const std::vector<int> &vPoints)
{
    if(vPoints.size() != 4)
    {
        throw "Size is not Four at addPointFourFromVtk ";
    }
    quadtetids.push_back(vPoints);
}

void Mesh3D::addTriangleFromVtk(const std::vector<int> &vIds)
{
}

void Mesh3D::addTetAndQuad(const std::vector<int> & vIds , const std::string &arg)
{
    if(vIds.size() != 4)
    {
        throw "Size is not Four at addTetAndQuad ";
    }
    Point_3* points[4];
    size_t   pointsID[4];
    for(int i=0; i<4; i++)
    {
       points[i]    = vAllVertices.at(vIds.at(i));
       pointsID[i]  = mPoint3toID[points[i]];
    }
    if(arg == "Tet")
    {
        createTetrahedron(points,pointsID);
    }
    else
    {
        createQuad(points,pointsID);
    }

}

void Mesh3D::addPrismFromVtk(const std::vector<int> & vIds)
{

    if(vIds.size() != 6)
    {
        throw "Size is not Six at addPrismFromVtk ";
    }
    Point_3* points[6];
    size_t   pointsID[6];
    for(int i=0; i<6; i++)
    {
        points[i]    = vAllVertices.at(vIds.at(i));
        pointsID[i]  = mPoint3toID[points[i]];
    }
    createPrism(points,pointsID);
}

void Mesh3D::splitQuadTet(const std::vector<int> & vIds)
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
            addTetAndQuad(tetIds, "Tet");
        }
        if(vIds.at(i) == 9)
        {
            std::vector<int> quadIds = quadtetids.at(i);
            addTetAndQuad(quadIds, "Quad");
        }
    }
}

void Mesh3D::writeTet(const std::string &filename_)
{
    writeTetvtk(filename_,vAllVertices,vAllTetrahedron);
}

void Mesh3D::printContainersInfo()
{
    std::cout << "size of vAllVertices      : " <<vAllVertices.size() << std::endl;
    std::cout << "size of quad              : " <<vAllQuad.size() << std::endl;
    std::cout << "size of vAllTetrahedron   : " <<vAllTetrahedron.size() << std::endl;
    std::cout << "size of vAllPrism         : " <<vAllPrism.size() << std::endl;

}