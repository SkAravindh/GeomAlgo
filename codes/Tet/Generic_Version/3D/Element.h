#ifndef ELEMENT_H
#define ELEMENT_H
#include <string>
#include "Definition.h"



class Element
{
public:
    Element(Mesh3D *parent);
     virtual ~Element();
    virtual Point_3* getCorner(unsigned int id) const   = 0;
    virtual size_t getCornerId(unsigned int id) const   = 0;
    virtual std::string getType() const                 = 0;
    virtual void setPolygonId(size_t id)                = 0;
    virtual void setPolygonStatus(bool flag)            = 0;
    virtual size_t getPolygonId() const                 = 0;
    virtual bool getPolygonStatus() const               = 0;
    virtual unsigned int getCornerSize() const          = 0;
    void setElementID(size_t id);
    size_t getElementID() const;

private:
    size_t elementID    = -1;
protected:
    Mesh3D* parentMesh       = nullptr;
};

#endif