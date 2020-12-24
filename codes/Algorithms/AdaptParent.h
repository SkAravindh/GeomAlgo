#ifndef ADAPTPARENT_H
#define ADAPTPARENT_H
#include "AdaptivieTriangle.h"
class Parent{
public:
    Parent(Triangle* a);
    Parent* getFirstN();
    Parent* getSecondN();
    Parent* getThirdN();
    int childrenSize();
    bool isempty();
    bool isRefined();
    void setRefineStatus(bool a);
    void setCurrlevel(int a);
    int getCurrlevel();

private:
    Parent* firstNeigh;
    Parent* secondNeigh;
    Parent* thirdNeigh;
    std::vector<Triangle*> sons;
    bool refineStatus=false;
    int currentlevel;
    Triangle* root;
};




#endif