#include "drawmanager.h"

DrawManager::DrawManager(){}

QGraphicsItem* DrawManager::getPainter(Body* b){
    Shape* s = b->getShape();
    QGraphicsItem* drawnShape;
    switch (s->getType()){
    case s->CIRCLE: {
         drawnShape = circlePainterBuilder(b);
         break;
    }
    case s->RECT: {
        drawnShape = rectPainterBuilder(b);
        break;
    }
    }
    return drawnShape;
}

QGraphicsItem* DrawManager::circlePainterBuilder(Body* b){
    Circle *c  = reinterpret_cast<Circle *>(b->getShape());

    return new CirclePainter(b->getPos(), c->getRadius());
}

QGraphicsItem* DrawManager::rectPainterBuilder(Body* b){
    Rectangle *r  = reinterpret_cast<Rectangle *>(b->getShape());
    QPointF p = b->getPos();

    return new RectPainter(p, QPointF(p.x() + r->getWidth(), p.y() + r->getHeight()));
}



//-----------------------------------------------------------------------------------------------------------//

CirclePainter::CirclePainter(QPointF center, double radius): center(center), radius(radius){}

void CirclePainter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(QPen(Qt::black, 3));

    painter->drawEllipse(center.x(), center.y(), radius, radius);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF CirclePainter::boundingRect() const{
    return QRectF(QPoint(center.x() - radius, center.y() + radius),
                  QPoint(center.x() + radius, center.y() - radius));
}


//-----------------------------------------------------------------------------------------------------------//

RectPainter::RectPainter(QPointF v1, QPointF v2): v1(v1), v2(v2){}

void RectPainter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(QPen(Qt::black, 3));

    painter->drawRect(v1.x(), v1.y(), v2.x(), v2.y());
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF RectPainter::boundingRect() const{
    return QRectF(QPoint(v1.x(), v1.y()),
                  QPoint(v2.x(), v2.y()));
}
