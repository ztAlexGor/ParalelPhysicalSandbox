#ifndef BODY_H
#define BODY_H

#include "shape.h"
#include "vector.h"
#include <QPointF>
#include <vector>

class Body{
    Shape* shape;
    QPointF pos;
    double mass;
    Vector velocity;
    std::vector<Vector> forces;
    Vector resForce;
    double acceleration;
    //double angle;
    //double angularVel;
    //double torque;
public:
    //Body();
    Body(Shape* shape);
    Body(Shape* shape, QPointF pos);
    Body(Shape* shape, QPointF pos, double mass);
    Shape* getShape();
    QPointF getPos();
};

#endif // BODY_H
