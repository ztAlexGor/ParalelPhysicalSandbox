#include "board.h"

Board::Board(float FPS){
    invFPS = 1.0 / FPS;
    QTimer* timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(process()));
    timer->start(6);
}

Body* Board::addObject(Shape* shape, QPointF pos){
    Body* body = new Body(shape, pos); //x, y
    this->objects.push_back(body);
    return body;
}

void Board::draw(){
    clear();

    for (int i = 0; i < objects.size(); i++){
        addItem(painter->getPainter(objects[i]));
    }
    //QDebug() << this->items();
}

void Board::process(){
    collisions.clear();

    for (int i = 0; i < objects.size(); i++){
        Body* a = objects[i];

        for (int j = i + 1; j < objects.size(); j++){

            Body* b = objects[j];

            if (a->isStatic() && b->isStatic())continue;

            Collision* colPair = new Collision(a, b);
            colPair->collisionManage();
            if (colPair->getCrossNum()){
                colPair->calculateTotalResilience(invFPS);
                collisions.push_back(colPair);
            }
        }
    }

    // Integrate forces
    for(int i = 0; i < objects.size(); i++)
        objects[i]->calculateVelocity(invFPS);

    // Solve collisions
    for(int j = 0; j < iterNum; j++)
      for(int i = 0; i < collisions.size(); i++)
        collisions[i]->fixCollision();

    // Integrate velocities
    for(int i = 0; i < objects.size(); i++)
        objects[i]->calculatePos(invFPS);

    // Correct positions
    for(int i = 0; i < collisions.size(); i++)
        collisions[i]->positionalCorrection();

    // Clear all forces
    for(int i = 0; i < objects.size(); i++)
        objects[i]->clearTemporary();

    draw();
}



