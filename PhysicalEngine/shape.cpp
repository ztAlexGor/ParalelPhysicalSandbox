#include "shape.h"

Shape::Shape(){}

AABB* Shape::getAABB(){
    return this->aabb;
}
