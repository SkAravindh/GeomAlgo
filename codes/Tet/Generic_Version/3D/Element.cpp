#include "Element.h"
#include "Mesh3D.h"

Element::~Element() noexcept
{}

Element::Element(Mesh3D *parent): parentMesh(parent)
{}

void Element::setElementID(size_t id)
{
    elementID = id;
}

size_t Element::getElementID() const
{
    return elementID;
}