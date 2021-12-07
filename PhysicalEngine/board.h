#ifndef BOARD_H
#define BOARD_H

#include "body.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include <vector>
#include <QGraphicsScene>

class Board: public QGraphicsScene{
    std::vector<Body*> objects;

public:
    Board();
    Body* addObject(Shape*);
    void draw();
};

#endif // BOARD_H
