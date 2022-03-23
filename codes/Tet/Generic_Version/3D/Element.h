#ifndef ELEMENT_H
#define ELEMENT_H
#include <typeinfo>
#include <string>
#include "Definition.h"



class Element
{
public:
    Element(Mesh3D *parent);
     virtual ~Element();
    virtual Point_3* getCorner(unsigned int id) const   = 0;
    virtual size_t getCornerId(unsigned int id) const   = 0;
    virtual size_t getPolygonId() const                 = 0;
    virtual std::string getType() const                 = 0;
    virtual unsigned int getCornerSize() const          = 0;
    size_t getElementID() const;
    virtual bool getPolygonStatus() const               = 0;
    virtual bool _equal(Element const & rhs) const;
    virtual bool _notequal(Element const & rhs) const;
    bool operator == (Element const & rhs) const;
    bool operator != (Element const & rhs) const;
    virtual void setPolygonId(size_t id)                = 0;
    virtual void setPolygonStatus(bool flag)            = 0;
    void setElementID(size_t id);

private:
    size_t elementID;
protected:
    Mesh3D* parentMesh       = nullptr;
};

#endif