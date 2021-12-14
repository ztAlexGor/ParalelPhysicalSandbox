#include "body.h"


Body::Body(Shape *shape): shape(shape){
    setMass(1);
}

Body::Body(Shape *shape, QPointF pos): shape(shape), pos(pos){
    this->shape = shape->copy();
    this->pos = pos;
    setMass(1);
    //  id = 0;
    //forces();
    resForce = QVector2D();
    velocity = QVector2D();

    resilience = 0.2;
    angle = 0;
    angularVel = 0;

    staticFriction = 0.5f;
    dynamicFriction = 0.3f;
    restitution = 0.2f;
    int gravityScale = 5.0f;
    gravity = QVector2D(0, 10.0f * gravityScale);
}

Body::Body(Shape *shape, QPointF pos, float mass){
    this->shape = shape->copy();
    this->pos = pos;
    setMass(mass);
    //id = 0;

    //forces();
    resForce = QVector2D();
    velocity = QVector2D();

    resilience = 0.2;
    angle = 0;
    angularVel = 0;

    staticFriction = 0.5f;
    dynamicFriction = 0.3f;
    restitution = 0.2f;

    int gravityScale = 5.0f;
    gravity = QVector2D(0, 10.0f * gravityScale);
}

Body *Body::copy(){
    Body* nb = new Body(shape, pos, mass);
    nb->setAngle(angle);
    return nb;
}

Shape* Body::getShape(){
    return shape;
}

QPointF Body::getPos(){
    return pos;
}

void Body::setMass(float m){
    mass = m;

    if (mass == 0){
        setStatic();
    }else {
        is_static = false;
        invMass = 1.0 / mass;
        inertia = mass * shape->getInertiaKoef();
        invInertia = 1.0 / inertia;
    }
}

void Body::setPos(QPointF pos){
    this->pos = pos;
}

void Body::addForce(QVector2D newForce){
    forces.push_back(newForce);
    resForce += newForce;
}

void Body::applyImpulse(QVector2D impulse, QVector2D contactVector){
    velocity += invMass * impulse;
    angularVel += invInertia * crossProduct(contactVector, impulse);
}

void Body::calculateVelocity(float time){
    if (isStatic())return;
    velocity += (resForce * invMass + gravity)*(time / 2.0);
}

void Body::calculatePos(float time){
    if (isStatic())return;

    pos += fromVecToPoint(velocity * time);
    angle += angularVel * time;//todo
    setAngle(angle);

    calculateVelocity(time);
}

void Body::clearTemporary(){
    resForce.setX(0);
    resForce.setY(0);
    forces.clear();
}

void Body::setAngle(float angle){
    this->angle = angle;
    shape->SetOrient(angle);
}

float Body::getResilience(){
    return resilience;
}

QVector2D Body::getVelocity(){
return velocity;
}

float Body::getAngularVel(){
    return angularVel;
}

float Body::getAngle(){
    return angle;
}

float Body::getInvMass(){
    return invMass;
}

float Body::getInvInertia(){
    return invInertia;
}

float Body::getSFriction(){
    return staticFriction;
}

float Body::getDFriction(){
    return dynamicFriction;
}

void Body::setStatic(){
    is_static = true;
    invMass = 0;
    inertia = 0;
    invInertia = 0;
}

bool Body::isStatic(){
    return is_static;
}

//void Body::setID(int id){
//    this->id = id;
//}

//int Body::getID(){
//    return id;
//}

float Body::crossProduct(const QVector2D a, const QVector2D b){
    return a.x() * b.y() - a.y() * b.x();
}

QPointF Body::fromVecToPoint(QVector2D v){
    return QPointF(v.x(), v.y());
}

QPointF Body::transform(QPointF a, QPointF o, float r){
    float x =   (a.x() - o.x()) * qCos(r) + (a.y() - o.y()) * qSin(r);
    float y = - (a.x() - o.x()) * qSin(r) + (a.y() - o.y()) * qCos(r);
    return QPointF(x, y);
}

QVector2D Body::transform(QVector2D v, float r){
    //return QVector2D(v.x() * qCos(r) - v.y() * qSin(r), v.x() * qSin(r) + v.y() * qCos(r));
    return QVector2D(v.x() * qCos(r) + v.y() * qSin(r), - v.x() * qSin(r) + v.y() * qCos(r));
}
