#ifndef BOARD_H
#define BOARD_H

#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "drawmanager.h"
#include "collision.h"
#include <vector>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
//#include "useful.h"

class Board: public QGraphicsScene{
    Q_OBJECT
    std::vector<Body*> objects;
    std::vector<Collision*> collisions;
    DrawManager* painter;
    float invFPS;
    int iterNum = 5;
public:
    Board(float FPS);
    Body* addObject(Shape*, QPointF);
    void draw();
public slots:
    void process();
};

#endif // BOARD_H
