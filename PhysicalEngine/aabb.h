#ifndef AABB_H
#define AABB_H

#include "vector.h"

class AABB{
    Vector leftBottom;
    Vector rightTop;
public:
    AABB();
    AABB(Vector, Vector);
    bool isIntersect(const AABB*);
};

#endif // AABB_H
