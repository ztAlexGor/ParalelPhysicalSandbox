#include "aabb.h"

AABB::AABB() {}

AABB::AABB(Vector leftBottom, Vector rightTop) {
    this->leftBottom = leftBottom;
    this->rightTop = rightTop;
}

bool AABB::isIntersect(const AABB* other){
    if (this->leftBottom.isGreater(other->rightTop) && other->leftBottom.isGreater(this->rightTop))return true;
    if (this->leftBottom.isGreater(other->rightTop) && other->leftBottom.isGreater(this->rightTop))return true;
    return false;
}
