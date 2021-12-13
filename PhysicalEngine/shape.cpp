#include "shape.h"

Shape::Shape():u(0){}

AABB* Shape::getAABB(){
    return this->aabb;
}

Mat2 Shape::getU(){
    return u;
}


void Shape::SetOrient( float radians ){
    u.Set( radians );
}
