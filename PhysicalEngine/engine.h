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
#include <thread>
#include <chrono>
#include <mutex>

class Engine{

    QVector<EngineObject*> allObjects;
    QVector<EngineObject*> interactionBodies;
    QVector<Collision*> collisions;
    float invFPS;
    int iterNum = 5;
    int expStatus;
    bool friction;
    std::mutex mu;
    std::chrono::system_clock::time_point startTime;
    std::chrono::duration<float> durationTime;
    bool isParalel;
    int threadCount;


    int numOfColl = 0;
public:
    enum RUN_STATUS { RUNNING = 0, PAUSED, STOPPED };

    Engine(float FPS);
    EngineObject* addObject(Shape* shape, QPointF pos, QString name);
    EngineObject* getByName(QString name);
    QVector<EngineObject*> getDrawnObj();
    int getExpStatus();
    QVector<EngineObject*> getObjList();
    void enableFriction(bool u);
    void setParallel(bool u);
    void setNumOfThreads(int a);
    bool getFriction();
    bool lifeCycle();
    void start();
    void stop();
    void pause();
    void resume();
    void clear();
    QString getMode();
    int getCollisionNumber();
    std::chrono::duration<float> getDurationTime();

private:
    bool checkIfExpEndes();
    void clearCollisions();
    void deleteUseless();
    void findCollisions();
    QVector<Collision*>* getAllCollisionPairs();
    QVector<QVector<Collision*>>* divideCollisionPairsByThreads();
    void paralelFindCollisions();
    void paralelFindCollisionsWithBuff();
    void paralelFindCollisionsWithOrderdBuf();
    bool process();
    bool paralelProcess();
    static void paralelFixCollision(QVector<Collision*> collisions);

    void test();
};


#endif // ENGINE_H
