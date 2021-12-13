#ifndef COLLISION_H
#define COLLISION_H

#include <QPointF>
#include <QVector2D>
#include <QDebug>
#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "mymat.h"
//#include "useful.h"

class Collision{
//    static int JumpTable[Shape::NUMBER_OF_TYPES][Shape::NUMBER_OF_TYPES] = {}{};
    Body* a;
    Body* b;
    QVector2D normal;
    float depth;
    QPointF crossPoints[2];
    int crossNum;
    float minResilience;
    float df;
    float sf;
private:
    void CircleWithCircle();
    void CircleWithPolygon();
    void CircleWithPolygon1();
    void PolygonWithPolygon();
public:
    Collision(Body* a, Body* b);
    void collisionManage();
    int getCrossNum();
    void calculateTotalResilience(float time);
    QVector2D calcRelativeVel(QPointF crossPoint);
    void fixCollision();
    void positionalCorrection();

    QPointF fromVecToPoint(QVector2D v);
    QVector2D fromPointToVec(QPointF p);
    float crossProduct(const QVector2D a, const QVector2D b);
    QVector2D crossProduct(const QVector2D a, float s);
    QVector2D crossProduct(float s, const QVector2D a);
    float FindAxisLeastPenetration(int *faceIndex, Rectangle *A, Rectangle *B, Body* Ab, Body* Bb);
    void FindIncidentFace(QVector2D *v, Rectangle *RefPoly, Rectangle *IncPoly, Body* Ab, Body* Bb, QVector2D referenceNormal);
    int Clip(QVector2D n, float c, QVector2D *face);
};



#endif // COLLISION_H
