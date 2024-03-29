#ifndef ENGINEOBJECT_H
#define ENGINEOBJECT_H
#include "body.h"
#include <QPen>
#include <QString>



class EngineObject{
    //static int idCounter;
    //int id;
    Body* body;
    QString name;
    bool enable;
    bool selected;
    QPen* pen;
    QColor* color;
public:
    EngineObject(Body* b, QString name);
    ~EngineObject();
    bool isEnable();
    void setEnable(bool u);
    void setPen(QPen* newPen);
    void setColor(QColor* newColor);
    QColor *getColor();
    Body* getBody();
    QPen* getPen();
    //int getID();
    QString getName();
    EngineObject *copy();
    void setSelect(bool s);
    bool isSelected();
};

#endif // ENGINEOBJECT_H
