#ifndef USEFUL_H
#define USEFUL_H

#include <QPointF>
#include <QVector2D>



QPointF fromVecToPoint(QVector2D v){
    return QPointF(v.x(), v.y());
}

QVector2D fromPointToVec(QPointF p){
    return QVector2D(p.x(), p.y());
}



float crossProduct(const QVector2D a, const QVector2D b){
    return a.x() * b.y() - a.y() * b.x();
}

QVector2D crossProduct(const QVector2D a, float s){
    return QVector2D(s * a.y(), -s * a.x());
}

QVector2D crossProduct(float s, const QVector2D a){
    return QVector2D(-s * a.y(), s * a.x());
}


#endif // USEFUL_H
