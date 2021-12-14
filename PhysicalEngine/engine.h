#ifndef ENGINE_H
#define ENGINE_H

#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "collision.h"
#include "engineobject.h"
#include <QVector>
#include <QString>


class Engine{
    enum RUN_STATUS { RUNNING = 0, PAUSED, STOPPED };

    QVector<EngineObject*> allObjects;
    QVector<EngineObject*> interactionBodies;
    QVector<Collision*> collisions;
    float invFPS;
    int iterNum = 5;
    int expStatus;
public:
    Engine(float FPS);
    EngineObject* addObject(Shape* shape, QPointF pos, QString name);
    EngineObject* getByName(QString name);
    QVector<EngineObject*> getDrawnObj();
    void lifeCycle();
    void process();
    void start();
    void stop();
    void pause();
    void resume();
};

#endif // ENGINE_H
