#include "Bbox_3.h"

Bbox_3::Bbox_3(){
    x_min=DBL_MAX;
    y_min=DBL_MAX;
    z_min=DBL_MAX;

    x_max=DBL_MIN;
    y_max=DBL_MIN;
    z_max=DBL_MIN;
}
Bbox_3::Bbox_3(double x_min, double y_min,double z_min, double x_max, double y_max ,double z_max): x_min(x_min), y_min(y_min), z_min(z_min), x_max(x_max), y_max(y_max), z_max(z_max){

}

void Bbox_3::add_coordinates(double x, double y, double z) {

    if(x < x_min){ x_min=x;}
    if(y < y_min) {y_min=y;}
    if(z < z_min) {z_min=z;}
    if(x > x_max) {x_max=x;}
    if(y > y_max) {y_max=y;}
    if(z > z_max) {z_max=z;}

}


double Bbox_3::xmin() const {
    return x_min;
}

double Bbox_3::ymin() const {
    return y_min;
}

double Bbox_3::zmin() const {
    return z_min;
}

double Bbox_3::xmax() const {
    return x_max;
}

double Bbox_3::ymax() const {
    return y_max;
}

double Bbox_3::zmax() const {
    return z_max;
}

bool Bbox_3::check_range(double Amin,double Amax,double Bmin,double Bmax) {
    return (Amax>=Bmin && Amin<=Bmax);
}

bool Bbox_3::do_overlap(const Bbox_3 &lhs, const Bbox_3 &rhs) {
    return(  check_range( lhs.xmin(), lhs.xmax(), rhs.xmin(), rhs.xmax()) &&
             check_range( lhs.ymin(), lhs.ymax(), rhs.ymin(), rhs.ymax()) &&
             check_range( lhs.zmin(), lhs.zmax(), rhs.zmin(), rhs.zmax())  );
}

bool Bbox_3::operator==(const Bbox_3 &rhs) const {

    return ( this->xmin()==rhs.xmin() && this->ymin()==rhs.ymin() && this->zmin()==rhs.zmin() &&
             this->xmax()==rhs.xmax() && this->ymax()==rhs.ymax() && this->zmax()==rhs.zmax() );
}

Bbox_3 Bbox_3::operator +(const Bbox_3 &rhs)  {
    if(rhs.xmin() < x_min) x_min=rhs.xmin();
    if(rhs.ymin() < y_min) y_min=rhs.ymin();
    if(rhs.zmin() < z_min) z_min=rhs.zmin();
    if(rhs.xmax() > x_max) x_max=rhs.xmax();
    if(rhs.ymax() > y_max) y_max=rhs.ymax();
    if(rhs.zmax() > z_max) z_max=rhs.zmax();
    return *this;
}

void Bbox_3::getbounds(double &a_xmin, double &a_ymin, double &a_zmin,double &a_xmax, double &a_ymax, double &a_zmax) const {
    a_xmin=x_min;
    a_ymin=y_min;
    a_zmin=z_min;
    a_xmax=x_max;
    a_ymax=y_max;
    a_zmax=z_max;
}


std::ostream &operator << (std::ostream &out, const Bbox_3 &rhs){
    out<<"xmin "<<rhs.xmin()<<" "<<"ymin "<<rhs.ymin()<<" "<<"zmin "<<rhs.zmin()<<" "<<"xmax "<<rhs.xmax()<<" "<<"ymax "<<rhs.ymax()<<" "<<"zmax "<<rhs.zmax();
    return out;
}