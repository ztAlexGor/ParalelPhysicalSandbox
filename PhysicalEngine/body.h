#ifndef BODY_H
#define BODY_H

#include "shape.h"

class Body{
    Shape* shape;
    double mass;

public:
    Body();
    Body(Shape* shape);
    Body(Shape* shape, double mass);
    Shape* getShape();
};

#endif // BODY_H
