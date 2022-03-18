#ifndef EDGE3D_H
#define EDGE3D_H
#include "Definition.h"


template<typename T, int D>
class Edge3D
{
public:
    Edge3D(const T &p0, const T &p1);
    ~Edge3D() = default;
    bool operator < (const Edge3D<T,D> &rhs) const;
    bool operator == (const Edge3D<T,D> & rhs) const;
    size_t hash() const;
private:
    T data[2];
};

template<typename T, int D>
Edge3D<T,D>::Edge3D (const T &p0, const T &p1)
{
    data[0] = p0;
    data[1] = p1;
//    if(*data[0] > *data[1]){
//        std::swap(data[0],data[1]);
//    }
   // std::cout << p0 << " " << p1 << std::endl;
}

template<typename T, int D>
bool Edge3D<T,D>::operator < (const Edge3D<T, D> &rhs) const
{
    if(*data[0] < *rhs.data[0]) return true;
    if(*data[0] > *rhs.data[0]) return false;
    return (*data[1] < *rhs.data[1]);
}

template<typename T, int D>
bool Edge3D<T,D>::operator==(const Edge3D<T, D> &rhs) const
{
    //return ( (*data[0] == *rhs.data[0] && *data[1] == *rhs.data[1]) );
    return ( ( (*data[0] == *rhs.data[0] && *data[1] == *rhs.data[1]) ) || ( (*data[0] == *rhs.data[1] && *data[1] == *rhs.data[0]) ) );
}

template<typename T, int D>
size_t Edge3D<T,D>::hash() const
{
    return ( data[0]->getID() + data[1]->getID() ) + ( data[0]->getID() * data[1]->getID() );
}

template<typename T, int D>
std::ostream& operator << (std::ostream& out , const Edge3D<T,D>& rhs)
{
    out << rhs.data[0] << std::endl;
    out << rhs.data[1] << std::endl;
    return out;
}

template<typename T, int D>
std::ostream& operator << (std::ostream& out , const Edge3D<T,D>* rhs)
{
    out << *rhs;
    return out;
}

template<typename T, int D>
struct HashIndex
{
    size_t operator()(const Edge3D<T,D> &rhs) const noexcept
    {
        return rhs.hash();
    }
};

#endif
