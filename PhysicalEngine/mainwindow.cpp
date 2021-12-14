#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    //setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);

    myFont.setPointSize(12);
    engine = new Engine(60);
    board = new Board();
    rectCounter = 1;
    circleCounter = 1;
    //board->setSceneRect(0, 0, 1500, 850);

    ui->graphicsView->setScene(board);
    //ui->graphicsView->setFixedSize(1500, 850);
    //ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(process()));
    timer->start(6);

    clearAttributeGrid();

    setDefaultScene();
    ui->listWidget->setCurrentRow(-1);
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::setDefaultScene(){
    addCircleToProject(new Circle(100.0), QPointF(1100, 150), 10);
    addCircleToProject(new Circle(35.0), QPointF(1000, 360), 9);
    addCircleToProject(new Circle(50.0), QPointF(60, 450), 10);

    addRectToProject(new Rectangle(240, 110), QPointF(200, 80), 40);
    addRectToProject(new Rectangle(200, 3), QPointF(150, 200), 0, 0.1);
    addRectToProject(new Rectangle(500, 3), QPointF(1130, 400), 0);
    addRectToProject(new Rectangle(450, 3), QPointF(670, 520), 0, -0.5);
    addRectToProject(new Rectangle(800, 3), QPointF(850, 800), 0);
    addRectToProject(new Rectangle(500, 3), QPointF(200, 650), 0, 0.5);
}

void MainWindow::addCircleToProject(Circle* c, QPointF pos, float mass, float ang){
    QString name = "circle " + QString::number(circleCounter++);

    EngineObject *o = engine->addObject(c, pos, name);
    o->getBody()->setMass(mass);
    o->getBody()->setAngle(ang);

    ui->listWidget->addItem(name);
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1 );
    clearAttributeGrid();
    openCircleMenu(o);
}

void MainWindow::addRectToProject(Rectangle * r, QPointF pos, float mass, float ang){
    QString name = "rectangle " + QString::number(rectCounter++);

    EngineObject *o = engine->addObject(r, pos, name);
    o->getBody()->setMass(mass);
    o->getBody()->setAngle(ang);

    ui->listWidget->addItem(name);
    ui->listWidget->setCurrentRow(ui->listWidget->count() - 1 );
    clearAttributeGrid();
    openRectMenu(o);
}

void MainWindow::process(){
    QListWidgetItem* selectedObj = ui->listWidget->currentItem();

    engine->lifeCycle();
    QVector<EngineObject*> objToDraw = engine->getDrawnObj();

    for (int i = 0; i<objToDraw.size(); i++){
        if (selectedObj != nullptr && objToDraw[i]->getName() == selectedObj->text()){
            objToDraw[i]->setSelect(true);
        }
    }
    board->draw(objToDraw);
}

void MainWindow::on_pushButton_start_clicked(){
    engine->start();
}

void MainWindow::on_pushButton_pause_clicked(){
    engine->pause();
}

void MainWindow::on_pushButton_stop_clicked(){
    engine->stop();
}

void MainWindow::on_pushButton_add_circle_clicked(){
    addCircleToProject(new Circle(100), QPointF(300, 500));
}

void MainWindow::on_pushButton_add_rectangle_clicked(){
    addRectToProject(new Rectangle(100, 50), QPointF(300, 600));
}




void MainWindow::attributePosX(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    obj->getBody()->setPos(QPointF(arg1, obj->getBody()->getPos().y()));
}

void MainWindow::attributePosY(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    obj->getBody()->setPos(QPointF(obj->getBody()->getPos().x(), arg1));
}

void MainWindow::attributeColor(int arg1){
    // todo !!!
}

void MainWindow::attributeWidth(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    Rectangle* r = reinterpret_cast<Rectangle*>(obj->getBody()->getShape());
    r->setWidht(arg1);
}

void MainWindow::attributeHeight(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    Rectangle* r = reinterpret_cast<Rectangle*>(obj->getBody()->getShape());
    r->setHeight(arg1);
}

void MainWindow::attributeRotation(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    obj->getBody()->setAngle((float)arg1 / 180.0 * 3.14159f);
}

void MainWindow::attributeRadius(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    Circle* c = reinterpret_cast<Circle*>(obj->getBody()->getShape());
    c->setRadius(arg1);
}


void MainWindow::clearAttributeGrid(){
    while (ui->grid_attribute->count() > 0){
        QLayoutItem* it = ui->grid_attribute->takeAt(0);
        delete it->widget();
        delete it;
    }
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item){
    clearAttributeGrid();

    EngineObject* obj = engine->getByName(item->text());
    if (obj->getBody()->getShape()->getType() == Shape::CIRCLE){
        openCircleMenu(obj);
    }else if (obj->getBody()->getShape()->getType() == Shape::RECT){
        openRectMenu(obj);
    }
}


void MainWindow::openCircleMenu(EngineObject* obj){
    QLabel* label = new QLabel();
    label->setFont(myFont);
    label->setText("Position X");
    ui->grid_attribute->addWidget((QWidget*)label, 0, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Position Y");
    ui->grid_attribute->addWidget((QWidget*)label, 1, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Radius");
    ui->grid_attribute->addWidget((QWidget*)label, 2, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Color");
    ui->grid_attribute->addWidget((QWidget*)label, 3, 0);

    // ////////////////////////////////////

    QSpinBox *spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(2000);
    spin->setValue(obj->getBody()->getPos().x());
    ui->grid_attribute->addWidget((QWidget*)spin, 0, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributePosX);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(1500);
    spin->setValue(obj->getBody()->getPos().y());
    ui->grid_attribute->addWidget((QWidget*)spin, 1, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributePosY);

    Circle* c = reinterpret_cast<Circle*>(obj->getBody()->getShape());
    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(500);
    spin->setValue(c->getRadius());
    ui->grid_attribute->addWidget((QWidget*)spin, 2, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeRadius);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(10000);
    //spin->setValue(obj->getBody()->getAngle());
    ui->grid_attribute->addWidget((QWidget*)spin, 3, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeColor);
}

void MainWindow::openRectMenu(EngineObject* obj){
    QLabel* label = new QLabel();
    label->setFont(myFont);
    label->setText("Position X");
    ui->grid_attribute->addWidget((QWidget*)label, 0, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Position Y");
    ui->grid_attribute->addWidget((QWidget*)label, 1, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Width");
    ui->grid_attribute->addWidget((QWidget*)label, 2, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Height");
    ui->grid_attribute->addWidget((QWidget*)label, 3, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Rotation");
    ui->grid_attribute->addWidget((QWidget*)label, 4, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Color");
    ui->grid_attribute->addWidget((QWidget*)label, 5, 0);

    // ////////////////////////////////////

    QSpinBox *spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(2000);
    spin->setValue(obj->getBody()->getPos().x());
    ui->grid_attribute->addWidget((QWidget*)spin, 0, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributePosX);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(1500);
    spin->setValue(obj->getBody()->getPos().y());
    ui->grid_attribute->addWidget((QWidget*)spin, 1, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributePosY);

    Rectangle* r = reinterpret_cast<Rectangle*>(obj->getBody()->getShape());
    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(1000);
    spin->setValue(r->getWidth());
    ui->grid_attribute->addWidget((QWidget*)spin, 2, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeWidth);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(1000);
    spin->setValue(r->getHeight());
    ui->grid_attribute->addWidget((QWidget*)spin, 3, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeHeight);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(10000);
    spin->setMinimum(-10000);
    spin->setValue(obj->getBody()->getAngle());
    ui->grid_attribute->addWidget((QWidget*)spin, 4, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeRotation);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(10000);
    //spin->setValue(obj->getBody()->getAngle());
    ui->grid_attribute->addWidget((QWidget*)spin, 5, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeColor);
}


