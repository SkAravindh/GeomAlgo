#ifndef _PTS_H_
#define _PTS_H_
#include <iostream>
#include <cfloat>
#include <tuple>

template<typename T, int D>
class Point3D
{
public:
    using coordsType         = T;
    static const int dim     = D;
public:
    Point3D();
    ~Point3D();
    Point3D(T x, T y );
    Point3D(T x, T y, T z );
    Point3D(T x, T y, T z , T u);
    Point3D (const Point3D<T,D> & rhs);
    void setID(size_t id_);
    void setAlive(bool flag);
    size_t getID();
    bool isAlive();
    T getCorners(int i);
    T & operator [] (int i);
    const T & operator[] (int i) const;
    Point3D<T,D> & operator = (const Point3D<T,D> & rhs);
    bool operator < (const Point3D<T,D> & rhs) const;
    bool operator > (const Point3D<T,D> & rhs) const;
    bool operator == (const Point3D<T,D> & rhs) const;
    bool operator != (const Point3D<T,D> & rhs) const;

private:
    T coordinates[D];
    bool status     = false;
    size_t id       = -1;

};

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
T Point3D<T,D>::getCorners(int i)
{
    return coordinates[i];
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
}

template<typename T, int D>
bool Point3D<T,D>::operator < (const Point3D<T, D> &rhs) const
{
    if (D == 3)
    {
        return std::make_tuple(this->coordinates[0], this->coordinates[1], this->coordinates[2]) <
               std::make_tuple(rhs[0], rhs[1], rhs[2]);
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
}

template<typename T, int D>
bool Point3D<T,D>::operator!=(const Point3D<T, D> &rhs) const
{
    if(D==3)
    {
        return ((this->coordinates[0] != rhs[0]) || (this->coordinates[1] != rhs[1]) || (this->coordinates[2] != rhs[2]));
    }
}

template<typename T, int D>
std::ostream & operator << (std::ostream &out, const Point3D<T,D> & rhs)
{
    for(int i=0; i<D; i++)
    {
        out << rhs[i] << " ";
    }
    return out;
}

template<typename T, int D>
std::ostream & operator << (std::ostream &out, const Point3D<T,D>* rhs)
{
    out << *rhs;
    return out;
}

template<typename T, int D>
struct ComparePoint3D
{
    bool operator()(const Point3D<T, D> *rhs, const Point3D<T, D> *lhs) const
    {
        return *rhs < *lhs;
    }
};

#endif
