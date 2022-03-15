#ifndef FACE3D_H
#define FACE3D_H
#include <cfloat>
#include <tuple>
#include <iostream>

template<typename T, int D>
class Face3D
{
public:
    Face3D();
    ~Face3D() = default;
    Face3D(T x,T y);
    Face3D(T x,T y, T z);
    Face3D(T x,T y, T z, T u);
    bool operator < (const Face3D<T,D> & rhs) const;
    bool operator == (const Face3D<T,D> &rhs) const;
    T getFaceCorners(int i);
    void arrangeids();
    Face3D<T,D>* getHface();
    void setoppface(Face3D<T,D> * f);

private:
    T coordinates[D];
    std::size_t ids[D];
    Face3D<T,D>* halfFace = nullptr;
};

template<typename T, int D>
Face3D<T,D>::Face3D()
{

}

template<typename T, int D>
Face3D<T,D>::Face3D(T x,T y, T z)
{
    coordinates[0] = x;
    coordinates[1] = y;
    coordinates[2] = z;

    for(int i=0; i<3; i++)
    {
        ids[i] = coordinates[i]->getID();
    }
    arrangeids();
}

template<typename T, int D>
T Face3D<T,D>::getFaceCorners(int i)
{
    return coordinates[i];
}

template<typename T, int D>
Face3D<T,D>* Face3D<T,D>::getHface()
{
    return halfFace;
}

template<typename T, int D>
void Face3D<T,D>::setoppface(Face3D<T,D> * f)
{
    if(halfFace != nullptr)
    {
        throw "not equal to null";
    }
   halfFace = f;
}

template<typename T, int D>
void Face3D<T, D>::arrangeids()
{
    for(int i=0; i<D; i++)
    {
        for(int j=i+1; j<D; j++)
        {
            if(ids[i] > ids[j])
            {
                std::size_t temp = ids[i];
                ids[i] = ids[j];
                ids[j] = temp;
            }
        }
    }
}

template<typename T, int D>
bool Face3D<T, D>::operator < (const Face3D<T, D> &rhs) const
{
    return std::make_tuple(ids[0],ids[1],ids[2]) < std::make_tuple(rhs.ids[0],rhs.ids[1],rhs.ids[2]);
}

template<typename T, int D>
bool Face3D<T,D>::operator == (const Face3D<T, D> &rhs) const
{
    return (ids[0]==rhs.ids[0] && ids[1]==rhs.ids[1] && ids[2]==rhs.ids[2]);
}

template<typename T, int D>
std::ostream& operator << (std::ostream& out ,  Face3D<T,D>& rhs)
{
    out << rhs.getFaceCorners(0) << std::endl;
    out << rhs.getFaceCorners(1) << std::endl;
    out << rhs.getFaceCorners(2) << std::endl;
    return out;
}


#endif