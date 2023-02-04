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
    timer->start(1);

    clearAttributeGrid();
    initScene();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::setDefaultScene(){
    setScene1();
}

void MainWindow::setScene1(){
    //вимикаємо силу тертя
    engine->enableFriction(false);

    //додаємо об'єкти до сцени
    addCircleToProject(false, new Circle(100.0), QPointF(1100, 150), 10);
    addCircleToProject(false, new Circle(35.0), QPointF(1000, 360), 9);
    addCircleToProject(false, new Circle(50.0), QPointF(60, 450), 10);

    addRectToProject(false, new Rectangle(240, 110), QPointF(200, 80), 40);
    addRectToProject(false, new Rectangle(200, 3), QPointF(150, 200), 0, 0.1);
    addRectToProject(false, new Rectangle(500, 3), QPointF(1130, 400), 0);
    addRectToProject(false, new Rectangle(450, 3), QPointF(670, 520), 0, -0.5);
    addRectToProject(false, new Rectangle(800, 3), QPointF(850, 800), 0);
    addRectToProject(false, new Rectangle(500, 3), QPointF(200, 650), 0, 0.5);
}


void MainWindow::setScene2(){
    //кількість об'єктів
    int n = 200;

    engine->enableFriction(true);

    for(int i = 0; i < n/2; i++){
        addRectToProject(false, new Rectangle(80, 40), QPointF(800, 350), 20);
        addCircleToProject(false, new Circle(35.0), QPointF(800, 450), 20);
    }
}

void MainWindow::setScene3(){
    //кількість об'єктів
    int n = 100;

    engine->enableFriction(true);

    for(int i = 0; i < n/12; i++){
        for (int j = 0; j < 6; j++){
            addRectToProject(false, new Rectangle(60, 30), QPointF(310 + j* 200, 150), 20);
            addCircleToProject(false, new Circle(30.0), QPointF(810, 200 + j * 80), 20);
        }
    }
}



void MainWindow::addCircleToProject(bool isMenu, Circle* c, QPointF pos, float mass, float ang){
    QString name = "circle " + QString::number(circleCounter++);

    EngineObject *o = engine->addObject(c, pos, name);
    o->getBody()->setMass(mass);
    o->getBody()->setAngle(ang);

    ui->listWidget->addItem(name);

    clearAttributeGrid();
    if (isMenu){
        openCircleMenu(o);
        ui->listWidget->setCurrentRow(ui->listWidget->count() - 1 );
    }
}

void MainWindow::addRectToProject(bool isMenu, Rectangle * r, QPointF pos, float mass, float ang){
    QString name = "rectangle " + QString::number(rectCounter++);

    EngineObject *o = engine->addObject(r, pos, name);
    o->getBody()->setMass(mass);
    o->getBody()->setAngle(ang);

    ui->listWidget->addItem(name);
    clearAttributeGrid();

    if (isMenu){
        openRectMenu(o);
        ui->listWidget->setCurrentRow(ui->listWidget->count() - 1 );
    }
}

void MainWindow::process(){
    QListWidgetItem* selectedObj = ui->listWidget->currentItem();

    if (!engine->lifeCycle()){
        on_pushButton_stop_clicked();
    }

    QVector<EngineObject*> objToDraw = engine->getDrawnObj();

    for (int i = 0; i < objToDraw.size(); i++){
        if (selectedObj != nullptr && objToDraw[i]->getName() == selectedObj->text()){
            objToDraw[i]->setSelect(true);
        }
    }
    board->draw(objToDraw);
    ui->time_label->setText(engine->getMode() + " time: " + QString::number(std::floor(engine->getDurationTime().count() * 100.0 + 0.5) / 100.0));
    ui->collision_label->setText("collision number: " + QString::number(engine->getCollisionNumber()));
    //board->drawTime();
    //delete selectedObj;

//    ui->time_label->setText(engine->getMode() + " time: " + QString::number(0));
//    ui->collision_label->setText("collision number: " + QString::number(0));

    objToDraw.clear();
}

void MainWindow::on_pushButton_start_clicked(){
    clearAttributeGrid();
    ui->pushButton_add_circle->setEnabled(false);
    ui->pushButton_add_rectangle->setEnabled(false);

    ui->pushButton_start->setEnabled(false);
    ui->pushButton_pause->setEnabled(true);
    ui->pushButton_stop->setEnabled(true);

    ui->Settings->setEnabled(false);
    engine->start();
}

void MainWindow::on_pushButton_pause_clicked(){
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    engine->pause();
}

void MainWindow::on_pushButton_stop_clicked(){
    ui->pushButton_add_circle->setEnabled(true);
    ui->pushButton_add_rectangle->setEnabled(true);

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);

    ui->Settings->setEnabled(true);
    engine->stop();
}

void MainWindow::on_pushButton_add_circle_clicked(){
    addCircleToProject(true, new Circle(100), QPointF(300, 500));
}

void MainWindow::on_pushButton_add_rectangle_clicked(){
    addRectToProject(true, new Rectangle(100, 50), QPointF(300, 600));
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

void MainWindow::attributeMass(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());
    obj->getBody()->setMass(arg1);
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

void MainWindow::staticAtribbute(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());

    if (arg1 == 1)obj->getBody()->setStatic();
    else obj->getBody()->setMass(1);
}

void MainWindow::visibleAtribbute(int arg1){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());

    obj->setEnable(arg1);
}

void MainWindow::clearAttributeGrid(){
    while (ui->grid_attribute->count() > 0){
        QLayoutItem* it = ui->grid_attribute->takeAt(0);
        delete it->widget();
        delete it;
    }
}

void MainWindow::initScene(){
    QVector<EngineObject*> objList = engine->getObjList();

    for (int i =0; i< objList.size(); i++){
        ui->listWidget->addItem(objList[i]->getName());
    }

    ui->actionFriction->setChecked(engine->getFriction());
    ui->pushButton_pause->setEnabled(false);
    ui->pushButton_stop->setEnabled(false);
    ui->Num_of_threadth_label->setEnabled(false);
    ui->spinBox_num_of_threads->setEnabled(false);
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item){
    clearAttributeGrid();

    if (engine->getExpStatus() == engine->STOPPED){
        EngineObject* obj = engine->getByName(item->text());
        if (obj->getBody()->getShape()->getType() == Shape::CIRCLE){
            openCircleMenu(obj);
        }else if (obj->getBody()->getShape()->getType() == Shape::RECT){
            openRectMenu(obj);
        }
    }
}

void MainWindow::attributeColor(){
    QListWidgetItem* item = ui->listWidget->currentItem();
    EngineObject* obj = engine->getByName(item->text());

    QColor* color = new QColor(QColorDialog::getColor(Qt::white, this, "Choose color"));
    if (color->isValid()){
        obj->setColor(color);
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
    label->setText("Mass");
    ui->grid_attribute->addWidget((QWidget*)label, 2, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Radius");
    ui->grid_attribute->addWidget((QWidget*)label, 3, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Color");
    ui->grid_attribute->addWidget((QWidget*)label, 4, 0);

    // ////////////////////////////////////
//    label = new QLabel();
//    label->setFont(myFont);
//    label->setText("PX");
//    ui->grid_attribute->addWidget((QWidget*)label, 0, 2);

//    label = new QLabel();
//    label->setFont(myFont);
//    label->setText("PX");
//    ui->grid_attribute->addWidget((QWidget*)label, 1, 2);

//    label = new QLabel();
//    label->setFont(myFont);
//    label->setText("KG");
//    ui->grid_attribute->addWidget((QWidget*)label, 2, 2);

//    label = new QLabel();
//    label->setFont(myFont);
//    label->setText("PX");
//    ui->grid_attribute->addWidget((QWidget*)label, 3, 2);

    QPushButton* button = new QPushButton();
    button->setText("Set Color");
    button->setFont(myFont);
    ui->grid_attribute->addWidget((QWidget*)button, 4, 1);
    connect(button, &QPushButton::clicked, this, &MainWindow::attributeColor);
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

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(1000);
    spin->setMinimum(1);
    spin->setValue(obj->getBody()->getMass());
    ui->grid_attribute->addWidget((QWidget*)spin, 2, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeMass);

    Circle* c = reinterpret_cast<Circle*>(obj->getBody()->getShape());
    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(500);
    spin->setValue(c->getRadius());
    ui->grid_attribute->addWidget((QWidget*)spin, 3, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeRadius);

//    spin = new QSpinBox();
//    spin->setFont(myFont);
//    spin->setMaximum(10000);
//    //spin->setValue(obj->getBody()->getAngle());
//    ui->grid_attribute->addWidget((QWidget*)spin, 4, 1);
//    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeColor);
    QCheckBox * check = new QCheckBox();
    check->setText("Static");
    check->setFont(myFont);
    check->setChecked(obj->getBody()->isStatic());
    ui->grid_attribute->addWidget((QWidget*)check, 5, 0);
    connect(check, &QCheckBox::clicked, this, &MainWindow::staticAtribbute);

    check = new QCheckBox();
    check->setText("Visible");
    check->setFont(myFont);
    check->setChecked(obj->isEnable());
    ui->grid_attribute->addWidget((QWidget*)check, 5, 1);
    connect(check, &QCheckBox::clicked, this, &MainWindow::visibleAtribbute);
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
    label->setText("Mass");
    ui->grid_attribute->addWidget((QWidget*)label, 2, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Width");
    ui->grid_attribute->addWidget((QWidget*)label, 3, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Height");
    ui->grid_attribute->addWidget((QWidget*)label, 4, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Rotation");
    ui->grid_attribute->addWidget((QWidget*)label, 5, 0);

//    QPushButton* button = new QPushButton();

//    QString s = "QPushButton {background-color: " + obj->getColor()->name() + ";}";
//    ui->pushButton->setStyleSheet("QPushButton {background-color: #A3C1DA; color: red;}");
//    button->setAutoFillBackground(true);
//    ui->grid_attribute->addWidget((QWidget*)button, 6, 0);

    label = new QLabel();
    label->setFont(myFont);
    label->setText("Color");
    ui->grid_attribute->addWidget((QWidget*)label, 6, 0);
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

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(1000);
    spin->setMinimum(1);
    spin->setValue(obj->getBody()->getMass());
    ui->grid_attribute->addWidget((QWidget*)spin, 2, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeMass);

    Rectangle* r = reinterpret_cast<Rectangle*>(obj->getBody()->getShape());
    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMinimum(3);
    spin->setMaximum(1000);
    spin->setValue(r->getWidth());
    ui->grid_attribute->addWidget((QWidget*)spin, 3, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeWidth);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMinimum(3);
    spin->setMaximum(1000);
    spin->setValue(r->getHeight());
    ui->grid_attribute->addWidget((QWidget*)spin, 4, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeHeight);

    spin = new QSpinBox();
    spin->setFont(myFont);
    spin->setMaximum(10000);
    spin->setMinimum(-10000);
    spin->setValue(obj->getBody()->getAngle()* 180 / 3.14);
    ui->grid_attribute->addWidget((QWidget*)spin, 5, 1);
    connect(spin, &QSpinBox::valueChanged, this, &MainWindow::attributeRotation);


    QPushButton* button = new QPushButton();
    button->setText("Set Color");
    button->setFont(myFont);
    ui->grid_attribute->addWidget((QWidget*)button, 6, 1);
    connect(button, &QPushButton::clicked, this, &MainWindow::attributeColor);



    QCheckBox * check = new QCheckBox();
    check->setText("Static");
    check->setFont(myFont);
    check->setChecked(obj->getBody()->isStatic());
    ui->grid_attribute->addWidget((QWidget*)check, 7, 0);
    connect(check, &QCheckBox::clicked, this, &MainWindow::staticAtribbute);

    check = new QCheckBox();
    check->setText("Visible");
    check->setFont(myFont);
    check->setChecked(obj->isEnable());
    ui->grid_attribute->addWidget((QWidget*)check, 7, 1);
    connect(check, &QCheckBox::clicked, this, &MainWindow::visibleAtribbute);
}

void MainWindow::on_actionFriction_triggered(){
    engine->enableFriction(ui->actionFriction->isChecked());
}

void MainWindow::on_actionSet_default_scene_triggered(){
    on_action_clearScene_triggered();
    setDefaultScene();
}

void MainWindow::on_action_clearScene_triggered(){
    engine->clear();
    ui->listWidget->clear();
    rectCounter = 1;
    circleCounter = 1;
}

void MainWindow::on_actionParalell_processing_triggered(){
    engine->setParallel(ui->actionParalell_processing->isChecked());

    if (engine->getMode() == "Parallel"){
        ui->Num_of_threadth_label->setEnabled(true);
        ui->spinBox_num_of_threads->setEnabled(true);
    }else{
        ui->Num_of_threadth_label->setEnabled(false);
        ui->spinBox_num_of_threads->setEnabled(false);
    }
}

void MainWindow::on_actionExit_triggered(){
    QApplication::quit();
}

void MainWindow::on_spinBox_num_of_threads_valueChanged(int arg1){
    engine->setNumOfThreads(arg1);
}

void MainWindow::setScene4(){
    engine->enableFriction(true);

    for(int i = 0; i < 100; i++){
        addRectToProject(false, new Rectangle(100, 50), QPointF(810, 350), 20);
    }
}

void MainWindow::setScene5(){
    //вмикаємо силу тертя
    engine->enableFriction(true);

    //додаємо вертикальні стінки
    addRectToProject(false, new Rectangle(300, 1200), QPointF(-130, 550), 0, 0.001);
    addRectToProject(false, new Rectangle(300, 1200), QPointF(1750, 550), 0, -0.001);

    //створюємо 100 кіл
    for(int i = 0; i < 1000; i++){
        addCircleToProject(false, new Circle(10.0), QPointF(800, 350), 20);
        //addCircleToProject(false, new Circle(4.0), QPointF(800, 250), 20);
    }

    //додаємо статичний квадрат посередині сцени
//    addRectToProject(false, new Rectangle(50, 50), QPointF(800, 420), 0, 0);
}
