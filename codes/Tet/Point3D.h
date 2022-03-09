#ifndef _PTS_H_
#define _PTS_H_
#include <iostream>
#include <cfloat>
#include <tuple>
template<typename T, int D>
class Point3D {
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
    T & operator [] (int i);
    const T & operator[] (int i) const;
    Point3D<T,D> & operator = (const Point3D<T,D> & rhs);
    bool operator < (const Point3D<T,D> & rhs) const;
    bool operator > (const Point3D<T,D> & rhs) const;
    bool operator == (const Point3D<T,D> & rhs) const;
    bool operator != (const Point3D<T,D> & rhs) const;
 //   friend std::ostream & operator << (std::ostream &out, const Point3D<T,D> & rhs);

private:
    T coordinates[D];
    bool status = false;
    size_t id    = -1;
};

template<typename T, int D>
struct ComparePoint3D
{
    bool operator()(const Point3D<T, D> *rhs, const Point3D<T, D> *lhs) const {
        return *rhs < *lhs;
    }
};

#endif
