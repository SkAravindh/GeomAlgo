#include "AdaptParent.h"

Parent::Parent(Triangle* a): root(a) {

}

Parent* Parent::getFirstN() {
    return firstNeigh;
}

Parent* Parent::getSecondN() {
    return secondNeigh;
}

Parent* Parent::getThirdN() {
    return thirdNeigh;
}

int Parent::childrenSize() {
    return sons.size();
}

bool Parent::isempty() {
    return sons.empty();
}

bool Parent::isRefined() {
    return refineStatus;
}

void Parent::setRefineStatus(bool a) {
    refineStatus=a;
}

void Parent::setCurrlevel(int a) {
    currentlevel=a;
}

int Parent::getCurrlevel() {
    return currentlevel;
}