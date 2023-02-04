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
#include <QColorDialog>
#include <QColor>

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
    void setScene1();
    void setScene2();
    void setScene3();
    void setScene4();
    void setScene5();
    void addCircleToProject(bool isMenu, Circle* c, QPointF pos, float mass = 5, float ang = 0);
    void addRectToProject(bool isMenu, Rectangle * r, QPointF pos, float mass = 10, float ang = 0);

    void openCircleMenu(EngineObject* obj);
    void openRectMenu(EngineObject* obj);
    void clearAttributeGrid();

    void initScene();
    void loadScene();
private slots:
    void process();
    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_add_circle_clicked();

    void on_pushButton_add_rectangle_clicked();

    void attributePosX(int arg1);
    void attributePosY(int arg1);
    void attributeMass(int arg1);
    void attributeColor();
    void attributeWidth(int arg1);
    void attributeHeight(int arg1);
    void attributeRotation(int arg1);
    void attributeRadius(int arg1);

    void on_listWidget_itemClicked(QListWidgetItem *item);


    void staticAtribbute(int arg1);
    void visibleAtribbute(int arg1);
    void on_actionFriction_triggered();

    void on_actionSet_default_scene_triggered();

    void on_action_clearScene_triggered();

    void on_actionParalell_processing_triggered();

    void on_actionExit_triggered();

    void on_spinBox_num_of_threads_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QTimer* timer;
    Engine* engine;
    Board* board;
    int rectCounter;
    int circleCounter;
    QFont myFont;
};


#endif // MAINWINDOW_H
