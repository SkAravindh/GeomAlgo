#ifndef FACE3D_H
#define FACE3D_H
#include <cfloat>
#include <tuple>
#include <vector>
#include <set>
#include <iostream>

template<typename T,  typename Tobj>
class Face3D
{
public:
    Face3D();
    ~Face3D() = default;
    Face3D(T x,T y, void* element);
    Face3D(T x,T y, T z,void* element);
    Face3D(T x,T y, T z, T u,void* element);
    bool operator < (const Face3D<T,Tobj> & rhs) const;
    bool operator == (const Face3D<T,Tobj> &rhs) const;
    T getFaceCorners(int i);
    void arrangeids();
    Face3D<T,Tobj>* getHalfFace();
    void setHalfFace(Face3D<T,Tobj> * f);
    Tobj getPolygon();
    friend std::ostream& operator << (std::ostream& out , const Face3D<T,Tobj>& rhs)
    {
        for(int i=0; i<rhs.coordinates.size(); i++)
        {
            out << rhs.coordinates[i] << " ";
        }
        out <<" and respective Ids are : ";
        for(int i=0; i<rhs.coordinates.size(); i++)
        {
            out << rhs.ids[i] << " ";
        }
        out << "\n";
        return out;
    }

private:
    std::vector<T> coordinates;
    std::vector< std::size_t> ids;
    Face3D<T,Tobj>* halfFace = nullptr;
    Tobj polygon;
};

template<typename T,typename Tobj>
Face3D<T,Tobj>::Face3D()
{

}

template<typename T, typename Tobj>
Face3D<T,Tobj>::Face3D(T x,T y, T z, void* element )
{
    coordinates.push_back(x);
    coordinates.push_back(y);
    coordinates.push_back(z);

    for(int i=0; i<3; i++)
    {
        ids.push_back(coordinates.at(i)->getID());
    }
    /*ids.push_back(x);
    ids.push_back(y);
    ids.push_back(z);*/
    polygon=reinterpret_cast<Tobj>(element);
    arrangeids();
}

template<typename T, typename Tobj>
Face3D<T,Tobj>::Face3D(T x,T y, T z, T u,void* element)
{
    coordinates.push_back(x);
    coordinates.push_back(y);
    coordinates.push_back(z);
    coordinates.push_back(u);

    for(int i=0; i<4; i++)
    {
        ids.push_back(coordinates.at(i)->getID());
    }
   /* ids.push_back(x);
    ids.push_back(y);
    ids.push_back(z);
    ids.push_back(u);*/
    polygon=reinterpret_cast<Tobj>(element);
    arrangeids();
}

template<typename T,  typename Tobj>
T Face3D<T,Tobj>::getFaceCorners(int i)
{
    return coordinates[i];
}

template<typename T,  typename Tobj>
Face3D<T,Tobj>* Face3D<T,Tobj>::getHalfFace()
{
    return halfFace;
}

template<typename T, typename Tobj>
Tobj Face3D<T,Tobj>::getPolygon()
{
    return polygon;
}

template<typename T,  typename Tobj>
void Face3D<T,Tobj>::setHalfFace(Face3D<T,Tobj> * f)
{
    if(halfFace != nullptr)
    {
        throw "not equal to null";
    }
   halfFace = f;
}

template<typename T,  typename Tobj>
void Face3D<T, Tobj>::arrangeids()
{
    for(int i=0; i<coordinates.size(); i++)
    {
        for(int j=i+1; j<coordinates.size(); j++)
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

template<typename T, typename Tobj>
bool Face3D<T, Tobj>::operator < (const Face3D<T,Tobj> &rhs) const
{
    if(this->ids.size() != rhs.ids.size())
    {
        std::set<std::size_t> used(this->ids.begin(), this->ids.end());
        int identical_count = 0;
        for(std::size_t ID : rhs.ids)
        {
            if(!used.insert(ID).second)
            {
                identical_count=identical_count+1;
            }
        }
        if(identical_count==3)
        {
            return false;
        }
    }
    if(this->ids.size()==4 && rhs.ids.size()==4)
    {
        return std::make_tuple(ids[0],ids[1],ids[2],rhs.ids[3]) < std::make_tuple(rhs.ids[0],rhs.ids[1],rhs.ids[2],rhs.ids[3]);
    }
    return std::make_tuple(ids[0],ids[1],ids[2]) < std::make_tuple(rhs.ids[0],rhs.ids[1],rhs.ids[2]);
}

template<typename T, typename Tobj>
bool Face3D<T,Tobj>::operator == (const Face3D<T,Tobj> &rhs) const
{
    return (ids[0]==rhs.ids[0] && ids[1]==rhs.ids[1] && ids[2]==rhs.ids[2]);
}

#endif