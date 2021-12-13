#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector.h"
#include <QVector2D>
#include <QPointF>
#include "shape.h"


class Circle: public Shape{
    float radius;
public:
    Circle(float);
    void setAABB() override;
    Shape* copy()const override;
    int getType()const override;
    //Vector getCenter()const;
    float getRadius()const;
    float getInertiaKoef()const override;
    void getProjection(QVector2D axis, QPointF startPoint, float &minProj, float &maxProj);
};

#endif // CIRCLE_H
