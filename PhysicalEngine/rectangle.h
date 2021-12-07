#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "vector.h"
#include "shape.h"
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>


class Rectangle: public Shape{
    //int numOfVertices;
    //int* vertices;
    Vector v1;
    Vector v2;
public:
    Rectangle(Vector, Vector);
    void setAABB() override;
    Shape* get() override;
    int getType() const override;
    //Vector* getNormals();
    Vector* getVertices();
    Vector getV1() const;
    Vector getV2() const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

#endif // RECTANGLE_H
