#include "Point.h"
#include "Mesh.h"

Point::Point() {

}

Point::Point(double x, double y, double z,Mesh* mesh): powner(mesh){

    coordinates[0]=x;
    coordinates[1]=y;
    coordinates[2]=z;
}

double Point::x() const {
    return coordinates[0];
}

double Point::y() const {
    return coordinates[1];
}

double Point::z() const {
    return coordinates[2];
}

Vector3 Point::vertexNormal() {
    return powner->getVertexNormal(this);
}

Point& Point::operator=(const Point &rhs)  {

    if(this == &rhs){
        return *this;
    }

    coordinates[0]=rhs.coordinates[0];
    coordinates[1]=rhs.coordinates[1];
    coordinates[2]=rhs.coordinates[2];
    return *this;
}

bool Point::operator < ( const Point &rhs) const {
    //std::cout<< "x compare "<<rhs.x()<<" "<<"y "<<rhs.y()<<" "<<"z "<<rhs.z()<<std::endl;
    return std::make_tuple(this->x(), this->y(), this->z()) < std::make_tuple(rhs.x(), rhs.y(), rhs.z()) ;
}

bool Point::operator > ( const Point &rhs) const {
    return std::make_tuple(this->x(), this->y(), this->z()) > std::make_tuple(rhs.x(), rhs.y(), rhs.z()) ;
}

bool Point::operator==(const Point &rhs) const {
    return (  (this->x()==rhs.x()) && (this->y()==rhs.y()) && (this->z()==rhs.z()) );
}


bool Point::operator!=(const Point &rhs) const {
    return (  (this->x()!=rhs.x()) || (this->y()!=rhs.y()) || (this->z()!=rhs.z()) );
}

Point* Point::operator+ (const Point &rhs) const {
    Point* p = new Point(rhs.x()+this->x(),rhs.y()+this->y(),rhs.z()+this->z(), nullptr);
    return p;
}

Point* Point::operator-(const Point &rhs) const {
    Point* p = new Point(this->x()-rhs.x(),this->y()-rhs.y(),this->z()-rhs.z(), nullptr);
    return p;
}