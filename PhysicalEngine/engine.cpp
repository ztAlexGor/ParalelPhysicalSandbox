#include "engine.h"

Engine::Engine(float FPS){
    invFPS = 1.0 / FPS;
    expStatus = STOPPED;
}

EngineObject* Engine::addObject(Shape* shape, QPointF pos, QString name){
    EngineObject* obj = new EngineObject(new Body(shape, pos), name);
    allObjects.push_back(obj);
    return obj;
}

EngineObject *Engine::getByName(QString name){
    if (expStatus == STOPPED){
        for (int i = 0; i < allObjects.size(); i++){
            if (allObjects[i]->getName() == name)return allObjects[i];
        }
    }else{
        for (int i = 0; i < interactionBodies.size(); i++){
            if (interactionBodies[i]->getName() == name)return interactionBodies[i];
        }
    }
    return nullptr;
}

QVector<EngineObject*> Engine::getDrawnObj(){
    if (expStatus == STOPPED){
        QVector<EngineObject*> enObj;
        for (int i = 0; i < allObjects.size(); i++){
            if (allObjects[i]->isEnable())
                enObj.append(allObjects[i]);
        }
        return enObj;
    }else {
//        QVector<EngineObject*> interactObj;
//        for (int i = 0; i < interactionBodies.size(); i++){
//            int id = interactionBodies[i]->getID();

//            for (int j = 0; j < allObjects.size(); j++){
//                if (allObjects[j]->getID() == id){
//                    interactObj.append(allObjects[j]);
//                }
//            }
//        }
//        return interactObj;
        return interactionBodies;
    }
}

void Engine::lifeCycle(){
    if (expStatus == RUNNING){
        //clearUnnecessary();
        process();
    }
}

void Engine::process(){
    collisions.clear();

    for (int i = 0; i < interactionBodies.size(); i++){
        Body* a = interactionBodies[i]->getBody();

        for (int j = i + 1; j < interactionBodies.size(); j++){

            Body* b = interactionBodies[j]->getBody();

            if (a->isStatic() && b->isStatic())continue;

            Collision* colPair = new Collision(a, b);
            colPair->collisionManage();
            if (colPair->getCrossNum()){
                colPair->calculateTotalResilience(invFPS);
                collisions.push_back(colPair);
            }
        }
    }

    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->calculateVelocity(invFPS);

    for(int j = 0; j < iterNum; j++)
      for(int i = 0; i < collisions.size(); i++)
        collisions[i]->fixCollision();

    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->calculatePos(invFPS);

    for(int i = 0; i < collisions.size(); i++)
        collisions[i]->positionalCorrection();

    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->clearTemporary();
}

void Engine::start(){
    if (expStatus == RUNNING){
        return;
    }else if(expStatus == PAUSED){
        resume();
    }else{
        expStatus = RUNNING;

        for (int i = 0; i < allObjects.size(); i++){
            if (allObjects[i]->isEnable())
                interactionBodies.append(allObjects[i]->copy());
        }
    }
}

void Engine::stop(){
    expStatus = STOPPED;
    interactionBodies.clear();
}

void Engine::pause(){
    expStatus = PAUSED;
}

void Engine::resume(){
    expStatus = RUNNING;
}
