#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    board = new Board();

    Circle *c = new Circle(Vector(50.0, 40.0), 20.0);
    Body *b = board->addObject(c);

    c = new Circle(Vector(100, -20), 50);
    b = board->addObject(c);

    Rectangle* r = new Rectangle(Vector(-10, -70), Vector(30, 20));
    b = board->addObject(r);


    ui->graphicsView->setScene(board);
    //board->draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    board->draw();
}

