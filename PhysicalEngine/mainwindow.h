#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include "board.h"
#include "body.h"
#include "shape.h"
#include "rectangle.h"
#include "circle.h"
#include "vector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum RUN_STATUS { RUNNING = 0, PAUSED, STOPPED };

    int expStatus;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void fillTheSceneWithCircles();
    void fillTheSceneWithRect();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::MainWindow *ui;
    Board* board;
};
#endif // MAINWINDOW_H
