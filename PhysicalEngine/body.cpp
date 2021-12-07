#include "body.h"

Body::Body(){}

Body::Body(Shape *shape): shape(shape), mass(0){}

Body::Body(Shape *shape, double mass): shape(shape), mass(mass){}

Shape* Body::getShape(){
    return shape;
}
