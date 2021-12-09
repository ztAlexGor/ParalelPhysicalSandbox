#ifndef BOARD_H
#define BOARD_H

#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "drawmanager.h"
#include <vector>
#include <QGraphicsScene>

class Board: public QGraphicsScene{
    std::vector<Body*> objects;
    DrawManager* painter;
public:
    Board();
    Body* addObject(Shape*, QPointF);
    void draw();
    //void process();
};

#endif // BOARD_H
