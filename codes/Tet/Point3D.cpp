#include "../Tet/Point3D.h"



template<typename T, int D>
Point3D<T, D>::Point3D()
{
    for(int i=0; i<D; i++)
    {
        coordinates[i] = DBL_MAX;
    }
}

template<typename T, int D>
Point3D<T,D>::~Point3D()
{

}

template<typename T, int D>
Point3D<T,D>::Point3D(const Point3D<T,D> & rhs)
{
    for(int i=0; i<D; i++)
    {
        coordinates[i] = rhs[i];
    }
}

template<typename T, int D>
Point3D<T,D>::Point3D(T x, T y)
{
   // std::cout << "Constructor with 2 args called !! " << std::endl;
    coordinates[0] = x;
    coordinates[1] = y;
}

template<typename T, int D>
Point3D<T,D>::Point3D( T x, T y, T z)
{
   // std::cout << "Constructor with 3 args called !!! " << std::endl;
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
}

template<typename T, int D>
Point3D<T,D>::Point3D(T x, T y, T z, T u)
{
   // std::cout << "Constructor with 4 args called !!!! " << std::endl;
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;
    coordinates[3] = u;
}

template<typename T, int D>
void Point3D<T,D>::setID(size_t id_)
{
    id = id_;
}

template<typename T, int D>
void Point3D<T,D>::setAlive(bool flag)
{
    status = flag;
}

template<typename T, int D>
size_t Point3D<T,D>::getID()
{
    return id;
}

template<typename T, int D>
bool Point3D<T,D>::isAlive()
{
    return status;
}

template<typename T, int D>
Point3D<T,D> & Point3D<T,D>::operator = (const Point3D<T,D> & rhs)
{
    if(this == &rhs)
    {
        return *this;
    }
    for(int i=0; i<D; i++)
    {
        coordinates[i] = rhs[i];
    }
    return *this;
}

template<typename T, int D>
T & Point3D<T,D>::operator [] (int i)
{
     return coordinates[i];
}

template<typename T, int D>
const T & Point3D<T,D>::operator [] (int i) const
{
    return coordinates[i];
}

template<typename T, int D>
bool Point3D<T,D>::operator > (const Point3D<T, D> &rhs) const
{
    if(D==3)
    {
        return std::make_tuple(this->coordinates[0],this->coordinates[1],this->coordinates[2]) > std::make_tuple(rhs[0],rhs[1],rhs[2]);
    }
    else if(D==4)
    {
        return std::make_tuple(this->coordinates[0],this->coordinates[1],this->coordinates[2],this->coordinates[3]) > std::make_tuple(rhs[0],rhs[1],rhs[2],rhs[3]);
    }
}

template<typename T, int D>
bool Point3D<T,D>::operator < (const Point3D<T, D> &rhs) const
{
    if (D == 3)
    {
        return std::make_tuple(this->coordinates[0], this->coordinates[1], this->coordinates[2]) <
               std::make_tuple(rhs[0], rhs[1], rhs[2]);
    }
    else if(D==4)
    {
        return std::make_tuple(this->coordinates[0],this->coordinates[1],this->coordinates[2],this->coordinates[3]) < std::make_tuple(rhs[0],rhs[1],rhs[2],rhs[3]);
    }
}

template<typename T, int D>
bool Point3D<T,D>::operator==(const Point3D<T, D> &rhs) const
{
    if(D==3)
    {
        return ((this->coordinates[0] == rhs[0]) && (this->coordinates[1] == rhs[1]) &&
                (this->coordinates[2] == rhs[2]));
    }
    else if(D==4)
    {
        return ((this->coordinates[0] == rhs[0]) && (this->coordinates[1] == rhs[1]) &&
                (this->coordinates[2] == rhs[2]) && (this->coordinates[3] == rhs[3]));
    }
}

template<typename T, int D>
bool Point3D<T,D>::operator!=(const Point3D<T, D> &rhs) const
{
    if(D==3)
    {
        return ((this->coordinates[0] != rhs[0]) || (this->coordinates[1] != rhs[1]) || (this->coordinates[2] != rhs[2]));
    }
    else if(D==4)
    {
        return ((this->coordinates[0] != rhs[0]) || (this->coordinates[1] != rhs[1]) || (this->coordinates[2] != rhs[2]) ||  (this->coordinates[3] != rhs[3]) );
    }
}

template<typename T, int D>
std::ostream & operator << (std::ostream &out, const Point3D<T,D> & rhs)
{
    for(int i=0; i<D; i++)
    {
        out << rhs[i] << " ";
    }
    out << "\n";
    return out;
}

