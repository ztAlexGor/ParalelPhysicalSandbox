#include "circle.h"

Circle::Circle(double radius){
    this->radius = radius;
    setAABB();
}

void Circle::setAABB(){
    this->aabb = new AABB(Vector(-radius, - radius),
                          Vector(radius, radius));
}

Shape* Circle::copy()const{
    return new Circle(radius);
}

int Circle::getType()const{
    return CIRCLE;
}

double Circle::getRadius()const{
    return radius;
}


