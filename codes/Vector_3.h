#ifndef VECTOR_3_H
#define VECTOR_3_H
#include <iostream>
#include<math.h>


template<typename T> 
class Vector_3 {
public:
Vector_3();
Vector_3(T x, T y, T z );
//template<T>friend std::ostream & operator<< (std::ostream & out, const Vector_3<T> &rhs);

 T x() const;
 T y() const;
 T z() const;


 T lenght() const;
 T squared_length() const;
 inline Vector_3<T> make_unit_vector();

 inline const Vector_3<T> & operator +() const {
    return *this;
 }

inline  Vector_3<T> & operator -() const {
    e[0] = 0-e[0];
    e[1] = 0-e[1];
    e[2] = 0-e[2];
    return *this;
}

const T&  operator [] (int i) const;
T & operator [] (int i);

inline Vector_3<T>& operator + (const Vector_3<T> &rhs);
inline Vector_3<T>& operator - (const Vector_3<T> & rhs);
inline Vector_3<T>& operator = (const Vector_3<T> &rhs);
inline Vector_3<T>& operator * (  double t);
inline Vector_3<T>& operator += (const Vector_3<T> & rhs);
inline Vector_3<T>& operator -= (const Vector_3<T> & rhs);
inline Vector_3<T>& operator *= (const Vector_3<T> & rhs);
inline Vector_3<T>& operator /= (const Vector_3<T> & rhs);
inline Vector_3<T>& operator *= (double t);
inline Vector_3<T>& operator /= (double t);
inline Vector_3<T>& operator/(double t);

private:
T e[3];

};

template<typename T>
Vector_3<T>::Vector_3() {
e[0]=0;
e[1]=0;
e[2]=0;
}

template<typename T>
Vector_3<T>::Vector_3(T x, T y, T z ) {

e[0]=x;
e[1]=y;
e[2]=z;
}

template<typename T>
T Vector_3<T>::x() const {
    return e[0];
}

template<typename T>
T Vector_3<T>::y() const {
    return e[1];
}

template<typename T>
T Vector_3<T>::z() const {
    return e[2];
}

template<typename T>
Vector_3<T>& Vector_3<T>::operator + (const Vector_3<T> &rhs) {

    e[0] =e[0] + rhs.e[0];
    e[1] =e[1] + rhs.e[1];
    e[2] =e[2] + rhs.e[2];
    return *this;
}

template<typename T>
Vector_3<T>& Vector_3<T>::operator - (const Vector_3<T> & rhs) {

    e[0] = e[0]-rhs.e[0];
    e[1] = e[1]-rhs.e[1];
    e[2] = e[2]-rhs.e[2];
    return *this;
}

template<typename T>
Vector_3<T>& Vector_3<T>::operator * (  double t) {

    e[0] = e[0]*t;
    e[1] = e[1]*t;
    e[2] = e[2]*t;
    return *this;
}

template<typename T>
Vector_3<T>& Vector_3<T>::operator = (const Vector_3<T> &rhs) {

    if(this == &rhs){
        return *this;
    }

    e[0] = rhs.e[0];
    e[1] = rhs.e[1];
    e[2] = rhs.e[2];
    return *this;
}

template<typename  T>
Vector_3<T>& Vector_3<T>::operator/(double t) {

    e[0] = e[0]/t;
    e[1] = e[1]/t;
    e[2] = e[2]/t;
    return *this;
}

template<typename T>
T Vector_3<T>::lenght() const {
     return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
}

template<typename T>
T Vector_3<T>::squared_length() const {
    return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
}

template<typename T>
inline Vector_3<T> Vector_3<T>::make_unit_vector()  {

    float k = 1/ sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0]=e[0]*k;
    e[1]=e[1]*k;
    e[2]=e[2]*k;
    return *this;
}

template<typename T>
inline T dot(const Vector_3<T> &lhs, const Vector_3<T> &rhs) {
  //  std::cout<<lhs.x()<<" "<<rhs.x()<<" "<< lhs.y()<<" "<<rhs.y()<<" "<< lhs.z()<<" "<<rhs.z()<<std::endl; 
    return ( lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z() ); 
}

template<typename T>
inline Vector_3<T> cross_product(const Vector_3<T> &lhs, const Vector_3<T> &rhs) {

    T i=(rhs.z()*lhs.y() - rhs.y()*lhs.z() );
    T j=-(lhs.x()*rhs.z() - rhs.x()* lhs.z());
    T k= (lhs.x()*rhs.y() - rhs.x()* lhs.y());
    return Vector_3<T>(i,j,k);
}

template<typename T>
inline Vector_3<T> unit_vector(const Vector_3<T> rhs) {
    return  rhs/rhs.lenght();
}

template<typename T>
std::ostream & operator << (std::ostream &out, const Vector_3<T> & rhs) {
    out<<"x "<<rhs.x()<<" "<<"y "<<rhs.y()<<" "<<"z "<<rhs.z();
    return out;
} 

template<typename T>
const T & Vector_3<T>::operator[](int i) const {
    return e[i];
}

template<typename T>
T & Vector_3<T>::operator [] (int i) {
    return e[i];
}

template<typename T>
 inline Vector_3<T>& Vector_3<T>::operator+=(const Vector_3<T> & rhs) {

    e[0]=e[0]+rhs.e[0];
    e[1]=e[1]+rhs.e[1];
    e[2]=e[2]+rhs.e[2];
    return *this;
 }

 template<typename T>
 inline Vector_3<T>& Vector_3<T>::operator-=(const Vector_3<T> &rhs) {

     e[0]=e[0]-rhs.e[0];
     e[1]=e[1]-rhs.e[1];
     e[2]=e[2]-rhs.e[2];
     return *this;
 }

 template<typename T>
 inline Vector_3<T>& Vector_3<T>::operator*=(const Vector_3<T> &rhs) {

     e[0]=e[0]*rhs.e[0];
     e[1]=e[1]*rhs.e[1];
     e[2]=e[2]*rhs.e[2];
     return *this;
 }

 template<typename T>
 inline Vector_3<T>& Vector_3<T>::operator/=(const Vector_3<T> &rhs) {

    e[0] /=rhs.e[0];
    e[1] /=rhs.e[1];
    e[2] /=rhs.e[2];
    return *this;
 }

 template<typename T>
 inline Vector_3<T>& Vector_3<T>::operator*=( double t) {

     e[0] =e[0]*t;
     e[1] =e[1]*t;
     e[2] =e[2]*t;
     return *this;
 }

 template<typename T>
 inline Vector_3<T>& Vector_3<T>::operator/=( double t) {

     double k = 1.0/t;
     e[0] =e[0]*k;
     e[1] =e[1]*k;
     e[2] =e[2]*k;
     return *this;
 }  

 template<typename T>
 inline Vector_3<T> operator+(const Vector_3<T> &lhs, const Vector_3<T> &rhs) {
    return Vector_3<T>( lhs.x()+rhs.x(), lhs.y()+rhs.y(), lhs.z()+rhs.z() );
 }

 template<typename T>
 inline Vector_3<T> operator-(const Vector_3<T> &lhs, const Vector_3<T> &rhs) {
    return Vector_3<T>( lhs.x()-rhs.x(), lhs.y()-rhs.y(), lhs.z()-rhs.z() );
 }

 template<typename T>
 inline Vector_3<T> operator*(const Vector_3<T> &lhs, const Vector_3<T> &rhs) {
     return Vector_3<T>(lhs.x()*rhs.x(), lhs.y()*rhs.y(), lhs.z()*rhs.z() );
 }

 template<typename T>
 inline Vector_3<T> operator/(const Vector_3<T> &lhs, const Vector_3<T> &rhs) {
     return Vector_3<T>(lhs.x()/rhs.x(), lhs.y()/rhs.y(), lhs.z()/rhs.z() );
 }

 template<typename T>
 inline Vector_3<T> operator*(double  t, const Vector_3<T> & rhs) {
     return Vector_3<T>(t*rhs.x(), t*rhs.y(), t*rhs.z());
 }

  template<typename T>
 inline Vector_3<T> operator*(const Vector_3<T> & rhs, double  t) {
     return Vector_3<T>(t*rhs.x(), t*rhs.y(), t*rhs.z());
 }

 template<typename T>
 inline Vector_3<T> operator/(const Vector_3<T> & rhs, double t) {
  return Vector_3<T>(rhs.x()/t, rhs.y()/t, rhs.z()/t);
 }




#endif