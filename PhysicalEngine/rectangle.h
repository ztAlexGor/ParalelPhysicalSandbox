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
    double width;
    double height;
public:
    Rectangle(double, double);
    void setAABB() override;
    Shape* copy()const override;
    int getType() const override;
    //Vector* getNormals();
    Vector* getVertices();
    double getWidth()const;
    double getHeight()const;
};

#endif // RECTANGLE_H
