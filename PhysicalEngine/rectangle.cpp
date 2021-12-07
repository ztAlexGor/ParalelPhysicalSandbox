#include "rectangle.h"

Rectangle::Rectangle(Vector vertex1, Vector vertex2){
    this->v1 = vertex1;
    this->v2 = vertex2;
    setAABB();
}

void Rectangle::setAABB(){
    this->aabb = new AABB(this->v1, this->v2);
}

Shape* Rectangle::get(){
    return this;
}

int Rectangle::getType()const{
    return RECT;
}

Vector Rectangle::getV1()const{
    return v1;
}

Vector Rectangle::getV2()const{
    return v2;
}

Vector* Rectangle::getVertices(){
    Vector* dots = new Vector[4];
    dots[0] = v1;
    dots[1] = Vector(v1.X, v2.Y);
    dots[2] = v2;
    dots[3] = Vector(v2.X, v1.Y);
    return dots;
}

//Vector* Rectangle::getNormals(){
    //Vector* v = getVertices();
    //for (int i = 0; i < 4; i++){
    //
    //}

//}


void Rectangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(QPen(Qt::black, 3));

    painter->drawRect(v1.X, v1.Y, v2.X, v2.Y);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF Rectangle::boundingRect() const{
    return QRectF(QPoint(v1.X, v1.Y),
                  QPoint(v2.X, v2.Y));
}
