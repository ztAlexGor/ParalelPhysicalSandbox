#ifndef SHAPE_H
#define SHAPE_H
#include "aabb.h"
#include "mymat.h"

class Shape{
protected:
    AABB* aabb;
    virtual void setAABB() = 0;
    Mat2 u;
public:
    enum types{CIRCLE, RECT, NUMBER_OF_TYPES};

    Shape();
    AABB* getAABB();
    virtual int getType()const = 0;
    virtual Shape* copy()const = 0;
    virtual float getInertiaKoef()const = 0;
    Mat2 getU();
    void SetOrient(float radians);
};

#endif // SHAPE_H
