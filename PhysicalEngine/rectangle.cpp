#include "rectangle.h"

Rectangle::Rectangle(double w, double h){
    width = w;
    height = h;
    setAABB();
}

void Rectangle::setAABB(){
    aabb = new AABB(Vector(0, 0), Vector(width, height));
}

Shape* Rectangle::copy()const{
    return new Rectangle(width, height);
}

int Rectangle::getType()const{
    return RECT;
}

double Rectangle::getWidth()const{
    return width;
}

double Rectangle::getHeight()const{
    return height;
}

Vector* Rectangle::getVertices(){
    Vector* dots = new Vector[4];
    dots[0] = Vector(0, 0);
    dots[1] = Vector(width, 0);
    dots[2] = Vector(0, height);
    dots[3] = Vector(width, height);
    return dots;
}

//Vector* Rectangle::getNormals(){
    //Vector* v = getVertices();
    //for (int i = 0; i < 4; i++){
    //
    //}

//}



