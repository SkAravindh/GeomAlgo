#include "Element.h"
#include "Mesh3D.h"

Element::~Element() noexcept
{}

Element::Element(Mesh3D *parent)
{
    parentMesh = parent;
    elementID  = -1;
}

void Element::setElementID(size_t id)
{
    elementID = id;
}

size_t Element::getElementID() const
{
    return elementID;
}

bool Element::_equal(Element const & rhs) const
{
    if(typeid(*this) != typeid(rhs)) return false;
    return true;
}

bool Element::_notequal(const Element &rhs) const
{
    if(typeid(*this) != typeid(rhs)) return false;
    return true;
}

bool Element::operator==(const Element &rhs) const
{
    return this->_equal(rhs);
}

bool Element::operator !=(const Element &rhs) const
{
    return this->_notequal(rhs);
}