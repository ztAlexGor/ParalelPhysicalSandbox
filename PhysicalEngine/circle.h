#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector.h"
#include "shape.h"


class Circle: public Shape{
    double radius;
public:
    Circle(double);
    void setAABB() override;
    Shape* copy()const override;
    int getType()const override;
    //Vector getCenter()const;
    double getRadius()const;
};

#endif // CIRCLE_H
