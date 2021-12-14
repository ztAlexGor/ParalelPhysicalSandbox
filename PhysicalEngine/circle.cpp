#include "circle.h"

Circle::Circle(float radius){
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

float Circle::getRadius()const{
    return radius;
}

void Circle::setRadius(float r){
    radius = r;
    // todo !!! aabb
}

float Circle::getInertiaKoef() const{
    return radius * radius;
}

void Circle::getProjection(QVector2D axis, QPointF startPoint, float &minProj, float &maxProj){
//    float x = (axis.y()*axis.y()*startPoint.x() - startPoint.y()*axis.x()*axis.y())
//                /(axis.x()*axis.x() * axis.y()*axis.y());
//    float y = -axis.x()/axis.y() * x;
//    float dist = x*x + y*y;
    QVector2D cent(-startPoint);
    float dist = QVector2D::dotProduct(cent, axis);
    minProj = dist - radius;
    maxProj = dist + radius;
}


