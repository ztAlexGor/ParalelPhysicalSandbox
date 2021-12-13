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
    w.showMaximized();

    return a.exec();
}

