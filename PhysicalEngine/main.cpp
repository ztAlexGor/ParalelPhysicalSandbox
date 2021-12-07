#include "mainwindow.h"

#include <QApplication>
#include <QGraphicsView>
#include "board.h"
#include "body.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "vector.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Board board;
    //Circle c(Vector(50, 40), 20);
    //Body *b = board.addObject(&c);//, 40, 55
    //
    //Rectangle r(Vector(10, 10), Vector(30, 20));
    //b = board.addObject( &r);//, 40, 55
    //
    //board.draw();
    //board.update(0, 0, 300, 300);

    return a.exec();
}
