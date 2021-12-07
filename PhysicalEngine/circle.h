#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector.h"
#include "shape.h"
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

class Circle: public Shape{
    Vector center;
    double radius;
public:
    Circle(Vector, double);
    void setAABB() override;
    Shape* get() override;
    int getType()const override;
    Vector getCenter()const;
    double getRadius()const;
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

#endif // CIRCLE_H
