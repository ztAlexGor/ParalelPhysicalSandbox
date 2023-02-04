#include "engine.h"

Engine::Engine(float FPS){
    invFPS = 1.0 / FPS;
    expStatus = STOPPED;
    friction = true;
    isParalel = false;
    durationTime = durationTime.zero();
    threadCount = 6;
}

EngineObject* Engine::addObject(Shape* shape, QPointF pos, QString name){
    EngineObject* obj = new EngineObject(new Body(shape, pos), name);
    allObjects.push_back(obj);
    return obj;
}

EngineObject *Engine::getByName(QString name){
    if (expStatus == STOPPED){
        for (int i = 0; i < allObjects.size(); i++){
            if (allObjects[i]->getName() == name)return allObjects[i];
        }
    }else{
        for (int i = 0; i < interactionBodies.size(); i++){
            if (interactionBodies[i]->getName() == name)return interactionBodies[i];
        }
    }
    return nullptr;
}

QVector<EngineObject*> Engine::getDrawnObj(){
    if (expStatus == STOPPED){
        QVector<EngineObject*> enObj;
        for (int i = 0; i < allObjects.size(); i++){
            if (allObjects[i]->isEnable())
                enObj.append(allObjects[i]);
        }
        return enObj;
    }else {
//        QVector<EngineObject*> interactObj;
//        for (int i = 0; i < interactionBodies.size(); i++){
//            int id = interactionBodies[i]->getID();

//            for (int j = 0; j < allObjects.size(); j++){
//                if (allObjects[j]->getID() == id){
//                    interactObj.append(allObjects[j]);
//                }
//            }
//        }
//        return interactObj;
        return interactionBodies;
    }
}

int Engine::getExpStatus(){
    return expStatus;
}

QVector<EngineObject *> Engine::getObjList(){
    return allObjects;
}

bool Engine::checkIfExpEndes(){
    for (auto i : interactionBodies){
        if (!i->getBody()->isStatic())return false;
    }
    return true;
}

bool Engine::getFriction(){
    return friction;
}

void Engine::enableFriction(bool u){
    friction = u;
}

void Engine::setParallel(bool u){
    isParalel = u;
    durationTime = durationTime.zero();
}

void Engine::setNumOfThreads(int a){
    threadCount = a;
}

std::chrono::duration<float> Engine::getDurationTime(){
    if (expStatus == RUNNING){
        return durationTime + (std::chrono::high_resolution_clock::now() - startTime);
    }
    else {
        return durationTime;
    }
}

QString Engine::getMode(){
    return isParalel ? "Parallel" : "Sequential";
}

int Engine::getCollisionNumber(){
    return numOfColl;
}

void Engine::start(){
    if (expStatus == RUNNING){
        return;
    }else if(expStatus == PAUSED){
        resume();
    }else{
        expStatus = RUNNING;

        for (int i = 0; i < allObjects.size(); i++){
            if (allObjects[i]->isEnable())
                interactionBodies.append(allObjects[i]->copy());
        }

        startTime = std::chrono::high_resolution_clock::now();
        durationTime = durationTime.zero();
        numOfColl = 0;
    }
}

void Engine::stop(){
    if (expStatus == RUNNING){
        durationTime += std::chrono::high_resolution_clock::now() - startTime;
    }

    if (expStatus != STOPPED){
        expStatus = STOPPED;

//        for (auto i : interactionBodies){
//            delete i;
//        }
        interactionBodies.clear();
    }

    if (durationTime.count() < 0.1)durationTime = durationTime.zero();
}

void Engine::pause(){
    expStatus = PAUSED;
    durationTime += std::chrono::high_resolution_clock::now() - startTime;;
}

void Engine::resume(){
    expStatus = RUNNING;
    startTime = std::chrono::high_resolution_clock::now();
}

void Engine::clear(){
    allObjects.clear();
    interactionBodies.clear();
    friction = true;
}

void Engine::clearCollisions(){
    for (auto i : collisions){
        delete i;
    }
    collisions.clear();
}

void Engine::deleteUseless(){
    interactionBodies.erase(std::remove_if(interactionBodies.begin(), interactionBodies.end(),
                                           [](auto x){
                                                QPointF p = x->getBody()->getPos();
                                                if (/*p.x() < -500 || p.x() > 1700 || */p.y() > 1200){
                                                    //delete x;
                                                    return true;
                                                }
                                                return false;
                                            }), interactionBodies.end());
}

bool Engine::lifeCycle(){
    //test();
    if (expStatus == RUNNING){
        deleteUseless();
        if (checkIfExpEndes()){
            this->stop();
            return false;
        }

        return process();
    }
    return true;
}

bool Engine::process(){

    //шукаємо усі можливі колізії та інформацію про них
    //залежно від режиму обираємо необхідну функцію
    if (isParalel){
        paralelFindCollisionsWithOrderdBuf();
        //paralelFindCollisionsWithBuff();
    }
    else{
        findCollisions();
    }

    //застосовуємо усі додаткові сили (наприклад, силу тяжіння)
    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->calculateVelocity(invFPS);

    //вирішуємо колізію шляхом надання імпульсу
    //повторюємо дію iterNum кількість разів
    for(int j = 0; j < iterNum; j++)
      for(int i = 0; i < collisions.size(); i++)
        collisions[i]->fixCollision();

    //оновлюємо позицію тіла за певний проміжок часу
    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->calculatePos(invFPS);

    //позиційна корекція
    for(int i = 0; i < collisions.size(); i++)
        collisions[i]->positionalCorrection();

    //очищуємо список колізій
    clearCollisions();
    return true;
}

void Engine::findCollisions(){
    for (int i = 0; i < interactionBodies.size(); i++){
        Body* a = interactionBodies[i]->getBody();

        for (int j = i + 1; j < interactionBodies.size(); j++){

            Body* b = interactionBodies[j]->getBody();

            //статичні тіла не взаємодіють один із одним
            if (a->isStatic() && b->isStatic())continue;

            //створюємо об'єкт класу Collision для пари (a, b)
            Collision* colPair = new Collision(a, b, friction);

            //шукаємо чи є колізія
            colPair->collisionManage();

            //якщо інсують точки перетину, значить колівзія відбулась
            if (colPair->getCrossNum()){
                //обраховуємо коєфіцієнт тертя
                colPair->calculateTotalResilience(invFPS);
                collisions.push_back(colPair);
            }else{
                //якщо колізії немає, то видаляємо цю пару
                delete colPair;
            }
        }
    }

    //підраховуємо загальну кількість колізій
    numOfColl += collisions.size();
}

void Engine::paralelFindCollisions(){
    //отримуємо список усіх мошжливих колізій
    QVector<Collision*>* allPairs = getAllCollisionPairs();

    //створюємо масив потоків
    QVector<std::thread*> threadPool;

    int size = allPairs->size();
    int startPoint = 0;

    //розділяємо всі задачі між потоками
    for (int i = 0; i < threadCount; i++){
        int endPoint = startPoint + size / threadCount + ((size % threadCount / (i + 1)) ? 1 : 0);

        //запускаємо новий поток
        threadPool.push_back(new std::thread([&](int sp, int ep){

            //поток оброблює лише свій інтервал задач
            for (int c = sp; c < ep; c++){

                //викликаємо алгоритм пошуку колізії
                (*allPairs)[c]->collisionManage();

                //якщо існують точки перетину, то колізія відбулась
                if ((*allPairs)[c]->getCrossNum()){

                    //рахуємо коєфіцієнти тертя
                    (*allPairs)[c]->calculateTotalResilience(invFPS);

                    //додаємо колізію в список колізій
                    //використовуючи мютекс для того, щоб жодна не загубилась
                    mu.lock();
                    collisions.push_back((*allPairs)[c]);
                    mu.unlock();
                }else{
                    //видаляємо пару, оскільки колізії немає
                    delete (*allPairs)[c];
                }
            }
        }, startPoint, endPoint));

        startPoint = endPoint;
    }

    //очікіємо завершення роботи кожного потоку
    for (std::thread* t : threadPool){
        t->join();
    }

    //підраховуємо загальну кількість колізій
    numOfColl += collisions.size();

    //очищуємо пам'ять
    delete allPairs;
}

void Engine::paralelFindCollisionsWithBuff(){
    //створюємо масив потоків
    QVector<std::thread*> threadPool;

    //отримуємо список задач для кожного потоку
    QVector<QVector<Collision*>>* threadBuffers = divideCollisionPairsByThreads();

    //запускаємо потоки
    for (int t = 0; t < threadCount; t++){
        threadPool.push_back(new std::thread([&](QVector<Collision*>& buf){

            //обробляємо усі об'єкти із буфера
            for (int c = 0; c < buf.size(); c++){

                //викликаємо алгоритм пошуку колізії
                buf[c]->collisionManage();

                //якщо існують точки перетину, то колізія відбулась
                if (buf[c]->getCrossNum()){
                    //ініціалізуємо колізію (обчислення різних коєфіцієнтів)
                    buf[c]->calculateTotalResilience(invFPS);
                }else{
                    //видаляємо пару без колізії
                    delete buf[c];

                    //в буфері помічаємо її як nullptr
                    buf[c] = nullptr;
                }
            }
        //використовуємо std::ref() для передачі параметра за посиланням
        }, std::ref((*threadBuffers)[t])));
    }

    //очікіємо завершення роботи кожного потоку
    for (std::thread* t : threadPool){
        t->join();
    }

    //усі знайдені колізії додаємо до списку колізій
    for (auto b : *threadBuffers){
        for (auto c : b){
            if (c)collisions.push_back(c);
        }
    }

    //підраховуємо загальну кількість колізій
    numOfColl += collisions.size();

    //очищуємо пам'ять
    delete threadBuffers;
    threadPool.clear();
}

void Engine::paralelFindCollisionsWithOrderdBuf(){
    //створюємо масив потоків
    QVector<std::thread*> threadPool;

    //створюємо буфер для кожного потоку
    QVector<QVector<Collision*>> threadBuffers;
    for (int i = 0; i < threadCount; i++){
        threadBuffers.push_back(QVector<Collision*>());
    }

    //отримуємо список усіх мошжливих колізій
    QVector<Collision*>* allPairs = getAllCollisionPairs();

    int size = allPairs->size();
    int startPoint = 0;

    //розподіляємо задачі між потоками
    for (int i = 0; i < threadCount; i++){
        int endPoint = startPoint + size / threadCount + ((size % threadCount / (i + 1)) ? 1 : 0);

        //запускаємо потоки
        threadPool.push_back(new std::thread([&](int sp, int ep, QVector<Collision*>& buf){

            //поток оброблює лише свій інтервал задач
            for (int c = sp; c < ep; c++){

                //викликаємо алгоритм пошуку колізії
                (*allPairs)[c]->collisionManage();

                //якщо існують точки перетину, то колізія відбулась
                if ((*allPairs)[c]->getCrossNum()){
                    //рахуємо коєфіцієнти тертя
                    (*allPairs)[c]->calculateTotalResilience(invFPS);

                    //додаємо колізію в буфер
                    buf.push_back((*allPairs)[c]);
                }else{
                    //видаляємо пару, оскільки колізії немає
                    delete (*allPairs)[c];
                }
            }
        //використовуємо std::ref() для передачі параметра за посиланням
        }, startPoint, endPoint, std::ref(threadBuffers[i])));
        startPoint = endPoint;
    }

    //очікіємо завершення роботи кожного потоку
    for (std::thread* t : threadPool){
        t->join();
    }

    //усі знайдені колізії додаємо до списку колізій
    for (auto b : threadBuffers){
        for (auto c : b){
            if (c)collisions.push_back(c);
        }
    }

    //підраховуємо загальну кількість колізій
    numOfColl += collisions.size();

    //очищуємо пам'ять
    delete allPairs;
    threadBuffers.clear();
    threadPool.clear();
}

QVector<Collision*>* Engine::getAllCollisionPairs(){

    //створюємо вектор для збереження усіх можливих пар
    QVector<Collision*>* allPairs = new QVector<Collision*>;

    //перебираєто усі варіанти
    for (int i = 0; i < interactionBodies.size(); i++){
        Body* a = interactionBodies[i]->getBody();
        for (int j = i + 1; j < interactionBodies.size(); j++){
            Body* b = interactionBodies[j]->getBody();

            //якщо обидва тіла не статичні, то додаємо нову пару
            if (!(a->isStatic() && b->isStatic())){
                allPairs->push_back(new Collision(a, b, friction));
            }
        }
    }
    return allPairs;
}

QVector<QVector<Collision*>>* Engine::divideCollisionPairsByThreads(){

    //створюємо буфер для кожного потоку
    QVector<QVector<Collision*>>* threadBuffers = new QVector<QVector<Collision*>>;

    for (int i = 0; i < threadCount; i++){
        threadBuffers->push_back(QVector<Collision*>());
    }

    int tp = 0;//лічильник потоків

    //розділяємо задачі між потоками
    //додаємо по одній до буферу конкретного потоку
    for (int i = 0; i < interactionBodies.size(); i++){
        Body* a = interactionBodies[i]->getBody();
        for (int j = i + 1; j < interactionBodies.size(); j++){
            Body* b = interactionBodies[j]->getBody();

            //якщо обидва тіла не статичні, то колізія можлива
            if (!(a->isStatic() && b->isStatic())){

                //додаємо задачу в буфер потоку
                (*threadBuffers)[tp % threadCount].push_back(new Collision(a, b, friction));
                tp++;
            }
        }
    }
    return threadBuffers;
}








bool Engine::paralelProcess(){
    //шукаємо усі можливі колізії та інформацію про них
    //paralelFindCollisions();
    //paralelFindCollisionsWithBuff();
    paralelFindCollisionsWithOrderdBuf();

    //застосовуємо усі додаткові сили (наприклад, силу тяжіння)
    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->calculateVelocity(invFPS);

    //вирішуємо колізію шляхом надання імпульсу
    //повторюємо дію iterNum кількість разів
    for(int j = 0; j < iterNum; j++){
        for(int i = 0; i < collisions.size(); i++)
            collisions[i]->fixCollision();
    }

    //оновлюємо позицію тіла за певний проміжок часу
    for(int i = 0; i < interactionBodies.size(); i++)
        interactionBodies[i]->getBody()->calculatePos(invFPS);

    //позиційна корекція
    for(int i = 0; i < collisions.size(); i++)
        collisions[i]->positionalCorrection();

    //    for(int i = 0; i < interactionBodies.size(); i++)
    //        interactionBodies[i]->getBody()->clearTemporary();
    //очищуємо список колізій
    clearCollisions();
    return true;
}



//void Engine::test(){
//    startTime = std::chrono::high_resolution_clock::now();
//    QVector<int>* v1 = new QVector<int>();

//    for (int i = 0; i < 1000000; i++){
//        v1->append(1);
//    }

//    std::chrono::duration<float> dt = std::chrono::high_resolution_clock::now() - startTime;
//    float t1 = dt.count();

//    startTime = std::chrono::high_resolution_clock::now();
//    QVector<int>* v2 = new QVector<int>();
//    v2->reserve(1000000);

//    for (int i = 0; i < 1000000; i++){
//        v2->append(1);
//    }

//    dt = std::chrono::high_resolution_clock::now() - startTime;
//    float t2 = dt.count();
//    int a = 0;
//}




//void Engine::paralelFindCollisions(){
//    QVector<std::thread*> threadPool;
//    //QVector<Collision*>** threadBuffers = new QVector<Collision*>*[threadCount];

//    QVector<Collision*> allPairs;
//    //allPairs.reserve((interactionBodies.size() - 1) * interactionBodies.size()/2);

//    for (int i = 0; i < interactionBodies.size(); i++){
//        Body* a = interactionBodies[i]->getBody();
//        for (int j = i + 1; j < interactionBodies.size(); j++){
//            Body* b = interactionBodies[j]->getBody();
//            if (!(a->isStatic() && b->isStatic())){
//                allPairs.append(new Collision(a, b, friction));
//            }
//        }
//    }

//    int size = allPairs.size();
//    int startPoint = 0;
//    for (int i = 0; i < threadCount; i++){
//        int endPoint = startPoint + size / threadCount + ((size % threadCount / (i + 1)) ? 1 : 0);

//        threadPool.append(new std::thread([&](int sp, int ep/*, int num*/){
//            //QVector<Collision*>* buffer = new QVector<Collision*>();
//            //buffer->reserve(ep - sp);
//            for (int c = sp; c < ep; c++){
//                allPairs[c]->collisionManage();
//                if (allPairs[c]->getCrossNum()){
//                    allPairs[c]->calculateTotalResilience(invFPS);
//                    //buffer->append(allPairs[c]);
//                    numOfColl++;
//                }else{
//                    //mu.lock();
//                    delete allPairs[c];
//                    allPairs[c] = nullptr;
//                    //mu.unlock();
//                }
//            }
//            //
//            //threadBuffers[num] = buffer;
//            //
//        }, startPoint, endPoint/*, i*/));
//        startPoint = endPoint;
//    }

//    for (int i = 0; i < threadCount; i++){
//        threadPool[i]->join();
//    }
////
////    int s = 0;
////    for (int i = 0; i < threadCount; i++){
////        s+= threadBuffers[i]->size();
////    }
////    collisions.reserve(s);

////    for (int i = 0; i < threadCount; i++){
////        //collisions.insert(collisions.end(), threadBuffers[i]->begin(), threadBuffers[i]->end());
////        for (auto c : *threadBuffers[i]){
////            collisions.append(c);
////        }
////    }
////    delete[] threadBuffers;


//    for(auto c : allPairs){
//        if (c)collisions.append(c);
//    }

//    for(int j = 0; j < iterNum; j++)
//        for(auto c : collisions)
//            c->fixCollision();

//    for(int i = 0; i < interactionBodies.size(); i++)
//        interactionBodies[i]->getBody()->calculatePos(invFPS);

//    for(auto c : collisions)
//        c->positionalCorrection();


//    allPairs.clear();
//}

//void Engine::paralelFixCollision(QVector<Collision*> collisions){
//    for(int i = 0; i < collisions.size(); i++)
//        collisions[i]->fixCollision();
//}
