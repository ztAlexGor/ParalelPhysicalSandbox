#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <QGraphicsItem>
#include <QPointF>
#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "engineobject.h"

class DrawManager{
    QGraphicsItem* circlePainterBuilder(Body* b);
    QGraphicsItem* rectPainterBuilder(Body* b);

    QGraphicsItem* circlePainterBuilder(EngineObject* obj);
    QGraphicsItem* rectPainterBuilder(EngineObject* obj);

public:
    DrawManager();
    QGraphicsItem* getPainter(Body* b);
    QGraphicsItem* getPainter(EngineObject* obj);
};



class CirclePainter: public QGraphicsItem{
    QPointF center;
    float radius;
    float thickness;
    QPen* pen;
public:
    CirclePainter(QPointF center, float radius, QPen* pen);
    CirclePainter(QPointF center, float radius, float t);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};


class RectPainter: public QGraphicsItem{
    QPointF v;
    float w;
    float h;
    float a;
    float thickness;
    QPen* pen;
public:
    RectPainter(QPointF v, float w, float h, float a, QPen* pen);
    RectPainter(QPointF v, float w, float h, float a, float t);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};





#endif // DRAWMANAGER_H
