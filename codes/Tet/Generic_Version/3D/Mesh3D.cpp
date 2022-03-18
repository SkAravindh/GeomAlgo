#include "Mesh3D.h"
#include "Polygons.h"
#include "Point3D.h"

Mesh3D::Mesh3D()
{
}

Mesh3D::Mesh3D(const std::string &meshName_) : meshName(meshName_)
{
}

Mesh3D::~Mesh3D() noexcept
{
    for(Point_3* p : vAllVertices)
    {
        delete p;
    }

    for(Element* e : allPolygons)
    {
        delete e;
    }
}

Point_3* Mesh3D::createPoint_3(const double x, const double y, const double z)
{
    Point_3 p(x,y,z);
    Point_3* ptr = createPoint_3(p);
    return ptr;
}

Point_3* Mesh3D::createPoint_3(const Point_3 &p)
{
    MPt3PtrIter it = mPoint3toPoint3.find(p);
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

void Mesh3D::registerPoint_3(Element *elements_)
{

    for(unsigned int i=0; i<elements_->getCornerSize(); i++)
    {
        Point_3* p = elements_->getCorner(i);
        mmPointtoElements.insert({p,elements_});
    }
}

void Mesh3D::registerEdge(Edge_3 obj, Element* element_)
{
    ummEdgetoElements.insert({obj, element_});
}

void Mesh3D::registerFace(Face_3 *obj, Element *element_)
{
    typedef std::map<Face_3,Face_3*>::iterator mF3ptrIter;
    mF3ptrIter iter = mHfacetoHface.find(*obj);
    if(iter == mHfacetoHface.end())
    {
        mHfacetoHface.insert({*obj,obj});
        allHalfFaces.push_back(obj);
    }
    else
    {
        Face_3 * current = iter->second;
        current->setHalfFace(obj);
        obj->setHalfFace(current);
        allHalfFaces.push_back(obj);
    }
}

Element* Mesh3D::createTetrahedron(Point_3 **tetPoints, size_t *tetIds)
{
    Element* tet = new TetElement::Tetrahedron(tetPoints,tetIds,this);
    tet->setPolygonId(tetCount);
    tet->setPolygonStatus(true);
    tetCount=tetCount+1;
    allPolygons.push_back(tet);
    return tet;
}

void Mesh3D::getBorderFaces()
{
    std::vector<Face_3*> borderFace;
    for(auto ele : allHalfFaces)
    {
        if(ele->getHalfFace()== nullptr)
        {
           borderFace.push_back(ele);
        }
    }
    std::cout <<"size of border face is  : "<< borderFace.size() << std::endl;
    std::set<Point_3> set_border;
    std::vector<Point_3> vec_border;
    for(auto ele : borderFace)
    {
        for(int i=0; i<3; i++)
        {
            set_border.insert(*ele->getFaceCorners(i));
        }
    }
    vec_border.assign(set_border.begin(),set_border.end());
    writevtkPoints("borderpts",vec_border);
}

const std::vector<Point_3*>& Mesh3D::getallVertices() const
{
    return vAllVertices;
}

const std::map<Point_3*, size_t,ComparePoint3D<double,3>>& Mesh3D::getmPoint3toID() const
{
    return mPoint3toID;
}

void Mesh3D::printContainerInfo()
{
    std::cout << "size of vAllVertices      : " <<vAllVertices.size() << std::endl;
    std::cout << "size of allPolygons       : " <<allPolygons.size() << std::endl;
    std::cout << "size of allHalfFaces       : " <<allHalfFaces.size() << std::endl;

}