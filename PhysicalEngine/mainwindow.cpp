#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    board = new Board(60);

    board->setSceneRect(0, 0, 1500, 850);

    ui->graphicsView->setScene(board);
    ui->graphicsView->setFixedSize(1500, 850);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    expStatus = PAUSED;



    fillTheSceneWithCircles();
    fillTheSceneWithRect();
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::fillTheSceneWithCircles(){
    //b = board->addObject(new Circle(100.0), QPointF(130.0, 0.0));
    //b->setMass(100);
    //b = board->addObject(new Circle(100.0), QPointF(200, 40));
    //b->setMass(10);
    Body *b  = board->addObject(new Circle(100.0), QPointF(1100, 150));
    b->setMass(10);
    b  = board->addObject(new Circle(35.0), QPointF(1000, 360));
    b->setMass(9);
    b  = board->addObject(new Circle(50.0), QPointF(60, 450));
    b->setMass(10);
}

void MainWindow::fillTheSceneWithRect(){
    Body *b = board->addObject(new Rectangle(240, 110), QPointF(200, 50));
    b->setMass(40);
    b = board->addObject(new Rectangle(200, 3), QPointF(150, 200));
    b->setAngle(0.1);
    b->setMass(0);
    b = board->addObject(new Rectangle(500, 3), QPointF(1130, 400));
    b->setMass(0);
    b = board->addObject(new Rectangle(500, 3), QPointF(650, 520));
    b->setAngle(-0.5);
    b->setMass(0);
    b = board->addObject(new Rectangle(800, 3), QPointF(850, 800));
    b->setMass(0);
    b = board->addObject(new Rectangle(500, 3), QPointF(200, 650));
    b->setAngle(0.5);
    b->setMass(0);
}

void MainWindow::on_pushButton_start_clicked(){
    expStatus = RUNNING;
}

void MainWindow::on_pushButton_pause_clicked(){
    expStatus = PAUSED;
}

void MainWindow::on_pushButton_stop_clicked(){
    expStatus = STOPPED;
}

