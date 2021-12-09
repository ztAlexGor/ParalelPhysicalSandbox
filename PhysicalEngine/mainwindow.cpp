#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    board = new Board();

    Body *b = board->addObject(new Circle(20.0), QPointF(50.0, 40.0));
    b = board->addObject(new Circle(50.0), QPointF(100, -20));
    b = board->addObject(new Rectangle(40, 110), QPointF(-10, -70));


    ui->graphicsView->setScene(board);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //board->process();
    board->draw();

}


