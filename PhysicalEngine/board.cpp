#include "board.h"

Board::Board()
{

}

Body* Board::addObject(Shape* shape){
    Body* body = new Body(shape); //x, y
    this->objects.push_back(body);
    return body;
}

void Board::draw(){
    for (int i = 0; i < objects.size(); i++){
        Shape* s = objects[i]->getShape();

        switch (s->getType()){
        case s->CIRCLE: {
            Circle *c  = reinterpret_cast<Circle *>(s);
            this->addItem(new Circle(c->getCenter(), c->getRadius()));
            break;
        }
        case s->RECT: {
            Rectangle *r  = reinterpret_cast<Rectangle *>(s);
            this->addItem(new Rectangle(r->getV1(), r->getV2()));
            break;
        }
        }
    }
}
