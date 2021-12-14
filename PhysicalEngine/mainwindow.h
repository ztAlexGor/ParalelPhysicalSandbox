#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QVector>
#include <QTimer>
#include <QString>
#include "board.h"
#include "body.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "drawmanager.h"
#include "engine.h"
#include "engineobject.h"
#include <QListWidgetItem>
#include <QLayoutItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDefaultScene();
    void addCircleToProject(Circle* c, QPointF pos, float mass = 5, float ang = 0);
    void addRectToProject(Rectangle * r, QPointF pos, float mass = 10, float ang = 0);

    void openCircleMenu(EngineObject* obj);
    void openRectMenu(EngineObject* obj);
    void clearAttributeGrid();
private slots:
    void process();
    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_add_circle_clicked();

    void on_pushButton_add_rectangle_clicked();

    void attributePosX(int arg1);
    void attributePosY(int arg1);
    void attributeColor(int arg1);
    void attributeWidth(int arg1);
    void attributeHeight(int arg1);
    void attributeRotation(int arg1);
    void attributeRadius(int arg1);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    enum RUN_STATUS { RUNNING = 0, PAUSED, STOPPED };
    QTimer* timer;
    Engine* engine;
    Board* board;
    int rectCounter;
    int circleCounter;
    QFont myFont;
};


#endif // MAINWINDOW_H
