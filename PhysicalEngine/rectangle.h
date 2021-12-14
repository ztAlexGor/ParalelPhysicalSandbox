#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>
#include <QVector>
#include <QVector2D>

class Rectangle: public Shape{
    //int numOfVertices;
    //int* vertices;
    float width;
    float height;
public:
    Rectangle(float, float);
    void setAABB() override;
    Shape* copy()const override;
    int getType() const override;
    QVector<QVector2D> getNormals();
    QVector<QVector2D> getEdges();
    QVector<QPointF> getVertices();
    float getWidth()const;
    float getHeight()const;
    void setWidht(float w);
    void setHeight(float h);
    float getInertiaKoef()const override;
    void getProjection(QVector2D axis, QPointF startPoint, float &minProj, float &maxProj);
    QPointF GetSupport(const QVector2D &dir);
};

#endif // RECTANGLE_H
