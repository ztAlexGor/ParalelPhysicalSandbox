#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <QGraphicsItem>
#include <QPointF>
#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"


class DrawManager{
    QGraphicsItem* circlePainterBuilder(Body* b);
    QGraphicsItem* rectPainterBuilder(Body* b);
public:
    DrawManager();
    QGraphicsItem* getPainter(Body* b);
};



class CirclePainter: public QGraphicsItem{
    QPointF center;
    double radius;
public:
    CirclePainter(QPointF center, double radius);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};


class RectPainter: public QGraphicsItem{
    QPointF v1;
    QPointF v2;
public:
    RectPainter(QPointF v1, QPointF v2);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};





#endif // DRAWMANAGER_H
