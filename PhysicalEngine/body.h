#ifndef BODY_H
#define BODY_H

#include "shape.h"
#include "vector.h"
#include <QPointF>
#include <vector>
#include <QVector2D>
//#include "engineobject.h"
//#include "useful.h"
#include <QtCore/qmath.h>

class Body{
    //int id;
    Shape* shape;
    QPointF pos;
    bool is_static;
    float mass;
    float invMass;
    float inertia;
    float invInertia;
    std::vector<QVector2D> forces;
    QVector2D resForce;
    QVector2D velocity;
    float resilience;
    float angle;
    float angularVel;
    float torque;
    float staticFriction;
    float dynamicFriction;
    float restitution;
public:
    QVector2D gravity;
    //Body();
    Body(Shape* shape);
    Body(Shape* shape, QPointF pos);
    Body(Shape* shape, QPointF pos, float mass);
    Body* copy();
    Shape* getShape();
    QPointF getPos();
    float getResilience();
    QVector2D getVelocity();
    float getAngularVel();
    float getAngle();
    float getMass();
    float getInvMass();
    float getInvInertia();
    float getSFriction();
    float getDFriction();
    void setVelocity(QVector2D newVel);
    void setMass(float m);
    void setPos(QPointF pos);
    void addForce(QVector2D newForce);
    void applyImpulse(QVector2D impulse, QVector2D contactVector);
    void calculatePos(float time);
    void calculateVelocity(float time);
    void clearTemporary();
    void setAngle(float angle);
    void setStatic();
    bool isStatic();
    float crossProduct(const QVector2D a, const QVector2D b);
    QPointF fromVecToPoint(QVector2D v);
    static QPointF transform(QPointF a, QPointF o, float r);
    static QVector2D transform(QVector2D v, float r);
//    void setID(int id);
//    int getID();
};

#endif // BODY_H
