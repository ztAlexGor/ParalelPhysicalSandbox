#include "engineobject.h"


//int EngineObject::idCounter = 0;

EngineObject::EngineObject(Body* b, QString name){
    //id = idCounter;
    //idCounter++;
    body = b;
    this->name = name;
    //body->setID(id);
    enable = true;
    selected = false;
    pen = new QPen(Qt::black, 4);
    color = new QColor(Qt::white);
}

EngineObject::~EngineObject(){
    delete pen;
    delete color;
}

Body *EngineObject::getBody(){
    return body;
}

bool EngineObject::isEnable(){
    return enable;
}

void EngineObject::setEnable(bool u){
    enable = u;
}

QPen* EngineObject::getPen(){
    return pen;
}

QString EngineObject::getName(){
    return name;
}

void EngineObject::setPen(QPen* newPen){
    pen = newPen;
}

void EngineObject::setColor(QColor *newColor){
    if (!newColor->isValid())throw 1;
    color = new QColor(*newColor);
}

QColor *EngineObject::getColor(){
    return color;
}

EngineObject* EngineObject::copy(){
    EngineObject* nEng = new EngineObject(body->copy(), name);
    nEng->setEnable(enable);
    nEng->setPen(pen);
    nEng->setColor(color);
    return nEng;
}

void EngineObject::setSelect(bool s){
    selected = s;
}

bool EngineObject::isSelected(){
    return selected;
}

//int EngineObject::getID(){
//    return id;
//}
