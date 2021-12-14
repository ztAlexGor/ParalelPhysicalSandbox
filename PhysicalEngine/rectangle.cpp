#include "rectangle.h"

Rectangle::Rectangle(float w, float h){
    width = w;
    height = h;
    setAABB();
}

void Rectangle::setAABB(){
    aabb = new AABB(Vector(0, 0), Vector(width, height));
}

Shape* Rectangle::copy()const{
    return new Rectangle(width, height);
}

int Rectangle::getType()const{
    return RECT;
}

float Rectangle::getWidth()const{
    return width;
}

float Rectangle::getHeight()const{
    return height;
}

void Rectangle::setWidht(float w){
    width = w;
    //todo !!! AABB
}

void Rectangle::setHeight(float h){
    height = h;
    //todo !!!
}

float Rectangle::getInertiaKoef() const{
    return width * height / 2;
}

QVector<QPointF> Rectangle::getVertices(){
    QVector<QPointF> dots;
//    dots.append(QPointF(0, 0));
//    dots.append(QPointF(width, 0));
//    dots.append(QPointF(width, height));
//    dots.append(QPointF(0, height));
    dots.append(QPointF(-width/2, -height/2));
    dots.append(QPointF(width/2, -height/2));
    dots.append(QPointF(width/2, height/2));
    dots.append(QPointF(-width/2, height/2));
    return dots;
}

QVector<QVector2D> Rectangle::getEdges(){
    QVector<QVector2D> edges;
    QVector<QPointF> dots = getVertices();

    for (int i = 0; i < dots.size() - 1; i++){
        edges.append(QVector2D(dots[i + 1] - dots[i]));
    }
    edges.append(QVector2D(dots[0] - dots[dots.size() - 1]));

    return edges;
}

QVector<QVector2D> Rectangle::getNormals(){
    QVector<QVector2D> edges = getEdges();
    QVector<QVector2D> normals;
    for (int i = 0; i < edges.size(); i++){
        QVector2D n(edges[i].y(), -edges[i].x());
        n.normalize();
        normals.append(n);
    }
    return normals;
}

void Rectangle::getProjection(QVector2D axis, QPointF startPoint, float& minProj, float& maxProj){
    QVector<QPointF> dots = getVertices();

    maxProj = -99999;
    minProj =  99999;

    for (int i = 0; i < dots.size(); i++){
//        float x = axis.x()*axis.y()*(dots[i].y() + axis.x()/axis.y()*dots[i].x())/(axis.x()*axis.x()+axis.y()*axis.y());
//        float y = x * axis.y()/axis.x();
//        float len = sqrt((startPoint.x() - x)*(startPoint.x() - x) + (startPoint.y() - y)*(startPoint.y() - y));
        QVector2D v(dots[i]-startPoint);
        float dist = QVector2D::dotProduct(v, axis);

        if (minProj > dist)minProj = dist;
        if (maxProj < dist)maxProj = dist;
    }
}


QPointF Rectangle::GetSupport(const QVector2D& dir){
    double bestProjection = -FLT_MAX;
    QPointF bestVertex;
    QVector<QPointF> dots = getVertices();

    for(int i = 0; i < dots.size(); i++){
        QPointF v = dots[i];
        double projection = QVector2D::dotProduct(QVector2D(v), dir);

        if(projection > bestProjection){
            bestVertex = v;
            bestProjection = projection;
        }
    }
    return bestVertex;
}




