#ifndef TOOLS_H
#define TOOLS_H
#include<iostream>
#include<unordered_map>
#include<map>
#include <memory>
#include "Edge3D.h"


template<class key, class value >
bool eraseKeyPairUMM(std::unordered_multimap<key ,value, HashIndex<Point_3*,2>> &umm, const key& k, const value& v)
{
    typedef typename std::unordered_multimap<key ,value, HashIndex<Point_3*,2>>::iterator IT;
    std::pair<IT,IT> range = umm.equal_range(k);
    std::vector<IT> to_del;
    IT iter = range.first;
    for(;iter!=range.second && iter->first==k; ++iter)
    {
        if(*iter->second == *v)
        {
            to_del.push_back(iter);
        }
    }
    std::cout <<"to_del :"<< to_del.size()<< std::endl;
    if(to_del.size() != 1) //never happen, just to make sure.
    {
        throw "size is not one";
    }
    for(IT &it : to_del)
    {
        umm.erase(it);
    }
    return !to_del.empty();
}

template<typename key, typename value>
bool eraseKeyPairMM(std::multimap<key, value> &mm, const key& k, const value& v)
{
    typedef typename std::multimap<key, value>::iterator IT;
    std::pair<IT,IT> range = mm.equal_range(k);
    std::vector<IT> to_del;
    IT iter = range.first;
    for(;iter!=range.second && iter->first==k; ++iter)
    {
        if(*iter->second == *v)
        {
            to_del.push_back(iter);
        }
    }
    std::cout <<"to_del :"<< to_del.size()<< std::endl;
    if(to_del.size() != 1) //never happen, just to make sure.
    {
        throw "size is not one";
    }
    for(IT &it : to_del)
    {
        mm.erase(it);
    }
    return !to_del.empty();
}

template<typename key, typename value>
bool eraseKeyPairMM(std::multimap<key, value , ComparePoint3D<double,3>> &mm, key& k, value& v)
{
    typedef typename std::multimap<key, value , ComparePoint3D<double,3>>::iterator IT;
    std::pair<IT,IT> range = mm.equal_range(k);
    std::vector<IT> to_del;
    IT iter = range.first;
    for(;iter!=range.second && *iter->first==*k; ++iter)
    {
        if(*iter->second == *v)
        {
            to_del.push_back(iter);
        }
    }
    //std::cout <<"to_del :"<< to_del.size()<< std::endl;
    if(to_del.size() != 1) //never happen, just to make sure.
    {
        throw "size is not one";
    }
    for(IT &it : to_del)
    {
        mm.erase(it);
    }
    return !to_del.empty();
}

#endif