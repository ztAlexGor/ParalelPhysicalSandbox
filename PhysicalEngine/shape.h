#ifndef SHAPE_H
#define SHAPE_H
#include "aabb.h"

class Shape{
protected:
    AABB* aabb;
    virtual void setAABB() = 0;

public:
    enum types{CIRCLE, RECT, T_COUNT};

    Shape();
    AABB* getAABB();
    virtual int getType()const = 0;
    virtual Shape* copy()const = 0;
};

#endif // SHAPE_H
