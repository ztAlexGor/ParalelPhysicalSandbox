#ifndef BOARD_H
#define BOARD_H

#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "drawmanager.h"
#include "engineobject.h"
#include <QGraphicsScene>


class Board: public QGraphicsScene{
    Q_OBJECT

    DrawManager* painter;
//    std::vector<Body*> objects;
//    std::vector<Collision*> collisions;
//    float invFPS;
//    int iterNum = 5;
public:
    Board();
    void clearScene();
    void draw(QVector<EngineObject*> obj);
    void drawTime(std::chrono::duration<float> time);

    //Board(float FPS);
    //Body* addObject(Shape*, QPointF);
    //void draw();
    //void process();
};

#endif // BOARD_H
