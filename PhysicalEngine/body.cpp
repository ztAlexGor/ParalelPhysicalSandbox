#include "body.h"

//Body::Body(){
//    shape =nullptr;
//    pos = QPointF();
//    mass = 0;
//    velocity = Vector();
//    resForce = Vector();
//    acceleration = 0;
//    Shape* shape;
//    QPointF pos;
//    double mass;
//    Vector velocity;
//    std::vector<Vector> forces;
//    Vector resForce;
//    double acceleration;
//    //double angle;
//    //double angularVel;
//    //double torque;
//}

Body::Body(Shape *shape): shape(shape), mass(0){}

Body::Body(Shape *shape, QPointF pos): shape(shape), pos(pos){}

Body::Body(Shape *shape, QPointF pos, double mass){
    this->shape = shape->copy();
    this->pos = pos;
    this->mass = mass;
    velocity = Vector();
    resForce = Vector();
    acceleration = 0;
    //forces;
    //double angle;
    //double angularVel;
    //double torque;
}

Shape* Body::getShape(){
    return shape;
}

QPointF Body::getPos(){
    return pos;
}
