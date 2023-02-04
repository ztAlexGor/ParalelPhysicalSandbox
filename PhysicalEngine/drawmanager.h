#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <QGraphicsItem>
#include <QPointF>
#include <QBrush>
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
    QColor* color;
public:
    CirclePainter(QPointF center, float radius, QPen* pen, QColor* color);
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
    QColor* color;
public:
    RectPainter(QPointF v, float w, float h, float a, QPen* pen, QColor* color);
    RectPainter(QPointF v, float w, float h, float a, float t);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};





#endif // DRAWMANAGER_H
