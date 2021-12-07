#include "circle.h"

Circle::Circle(Vector center, double radius){
    this->radius = radius;
    this->center = center;
    setAABB();
}

void Circle::setAABB(){
    this->aabb = new AABB(Vector(center.X - radius, center.Y - radius),
                          Vector(center.X + radius, center.Y + radius));
}

Shape* Circle::get(){
    return this;
}

int Circle::getType()const{
    return CIRCLE;
}

Vector Circle::getCenter()const{
    return center;
}

double Circle::getRadius()const{
    return radius;
}

void Circle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(QPen(Qt::black, 3));

    painter->drawEllipse(center.X, center.Y, radius, radius);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Circle::boundingRect() const{
    return QRectF(QPoint(center.X - radius, center.Y + radius),
                  QPoint(center.X + radius, center.Y - radius));
}
