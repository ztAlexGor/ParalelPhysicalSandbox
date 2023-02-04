#include "collision.h"

Collision::Collision(Body* a, Body* b, bool friction){
    this->a = a;
    this->b = b;
    this->friction = friction;
    depth = 0;
    crossNum = 0;
    minResilience = 0;
    df = 0;
    sf = 0;
}

int Collision::getCrossNum(){
    return crossNum;
}

QVector2D Collision::calcRelativeVel(QPointF crossPoint){
    QVector2D ra = fromPointToVec(crossPoint - a->getPos());
    QVector2D rb = fromPointToVec(crossPoint - b->getPos());

    return  (b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
             a->getVelocity() - crossProduct(a->getAngularVel(), ra));
}

void Collision::calculateTotalResilience(float time){

    minResilience = a->getResilience() < b->getResilience() ? a->getResilience() : b->getResilience();

    //обчислюємо коєф. тертя спокою та ковзання
    sf = sqrt(a->getSFriction() * b->getSFriction());
    df = sqrt(a->getDFriction() * b->getDFriction());

    for(int i = 0; i < crossNum; i++){
        QVector2D ra = fromPointToVec(crossPoints[i] - a->getPos());
        QVector2D rb = fromPointToVec(crossPoints[i] - b->getPos());

        QVector2D rv = (b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
                        a->getVelocity() - crossProduct(a->getAngularVel(), ra));

        //якщо на тіла не діє жодна сила окрім гравітації,
        //то не потрібно використовувати коєфіцієнт відшкодування
        if(rv.lengthSquared() < (time * a->gravity).lengthSquared( ) + 0.0001)
            minResilience = 0.0;
    }
}

void Collision::fixCollision(){
    //якщо обидва тіла статичні, то взаємодія не відбувається
    if(a->isStatic() && b->isStatic()){
        a->setVelocity(QVector2D(0, 0));
        b->setVelocity(QVector2D(0, 0));
        return;
    }

    //для кожної точки перетину
    for(int i = 0; i < crossNum; i++){
        QVector2D ra = fromPointToVec(crossPoints[i] - a->getPos());
        QVector2D rb = fromPointToVec(crossPoints[i] - b->getPos());

        //шукаємо швидкість B відносно А
        QVector2D rv = (b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
                        a->getVelocity() - crossProduct(a->getAngularVel(), ra));


        //швидкість B відносно А вздовж нормалі зіткнення
        float contactVel = QVector2D::dotProduct(rv, normal);

        //якщо тіла рухаються у різних напрямках, то не потрібно нічого змінювати
        if(contactVel > 0)
            return;

        float raCrossN = crossProduct(ra, normal);
        float rbCrossN = crossProduct(rb, normal);
        float invMassSum = a->getInvMass() + b->getInvMass() + raCrossN * raCrossN * a->getInvInertia()
                                                              + rbCrossN * rbCrossN * b->getInvInertia();

        //обчислюємо величину імпульсу
        float j = -(1.0f + minResilience) * contactVel;
        j /= invMassSum;
        j /= (float)crossNum;

        //застосовуємо імпульс до тіла A та B
        QVector2D impulse = normal * j;
        a->applyImpulse(-impulse, ra);
        b->applyImpulse(impulse, rb);

        //враховуємо силу тертя
        if (friction){
            rv = b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
                 a->getVelocity() - crossProduct(a->getAngularVel(), ra);

            //напрямок тангенціального імпульсу
            QVector2D t = rv - (normal * QVector2D::dotProduct(rv, normal));
            t.normalize();

            //величина тангенціального імпульсу
            float jt = - QVector2D::dotProduct(rv, t);
            jt /= invMassSum;
            jt /= (float)crossNum;

            //якщо величина імпульсу дуже мала, то просто ігноруємо її
            if(abs(jt) < 0.000001)return;

            //в залежності від типу тертя імпульс буде різним
            QVector2D tangentImpulse;
            if(abs(jt) < j * sf)
              tangentImpulse = t * jt;
            else
              tangentImpulse = t * (-j) * df;

            //застосовуємо імпульс тертя
            a->applyImpulse(-tangentImpulse, ra);
            b->applyImpulse(tangentImpulse, rb);
        }
    }
}

void Collision::positionalCorrection(){
    const float k_slop = 0.08f; //допуст проникнення (0.01; 0.1)
    const float percent = 0.5f; //відсоток проникнення, для корекції (0.2; 0.8)

    QVector2D correction = QVector2D(0, 0);

    float t = depth - k_slop;
    if (t > 0){
        correction = (t / (a->getInvMass() + b->getInvMass())) * normal * percent;
        a->setPos(fromVecToPoint(QVector2D(a->getPos()) - correction * a->getInvMass()));
        b->setPos(fromVecToPoint(QVector2D(b->getPos()) + correction * b->getInvMass()));
    }
}

void Collision::CircleWithCircle(){
    //отримуємо два круга
    Circle *c1  = reinterpret_cast<Circle *>(a->getShape());
    Circle *c2  = reinterpret_cast<Circle *>(b->getShape());

    //нормаль зіткнення
    normal = QVector2D(b->getPos() - a->getPos());

    //квадрат дистанції між центрами та сума радіусів
    float dist = normal.lengthSquared();
    float sumOfRadiuses = c1->getRadius() + c2->getRadius();

    //для оптимізації не використовуємо sqrt(), а порівнюємо квадрати відстаней
    if (dist >= sumOfRadiuses * sumOfRadiuses){//перетину немає
        crossNum = 0;
    }else if (dist == 0){//випадок коли центри кіл співпадають
        crossNum = 1;
        normal = QVector2D(1, 0);
        depth = c1->getRadius();
        crossPoints[0] = a->getPos();
    }else{//перетинаються
        dist = normal.length();
        crossNum = 1;
        normal /= dist;
        depth = sumOfRadiuses - dist;
        crossPoints[0] = fromVecToPoint(normal * c1->getRadius() + QVector2D(a->getPos()));
    }
}

void Collision::CircleWithPolygon1(){
    Circle *c  = reinterpret_cast<Circle *>(a->getShape());
    Rectangle *r  = reinterpret_cast<Rectangle *>(b->getShape());
    float penetration = 99999999.f;

    QVector<QVector2D> axcisList_old = r->getNormals();
    QVector<QVector2D> axcisList;

    for (int i = 0; i<axcisList_old.size(); i++){
        axcisList.append(Body::transform(axcisList_old[i], b->getAngle()));
    }

    if (abs(b->getPos().y() - 41.3468) < 0.001){
        int fsd = 0;
        int btu = fsd;
    }
    for(int i = 0; i < axcisList.size(); i++){
        float minP1, maxP1, minP2, maxP2;

        QPointF start = Body::transform(QPointF(0, 0), a->getPos(), a->getAngle());
        QVector2D newAxcis = Body::transform(axcisList[i], a->getAngle());
        c->getProjection(newAxcis, start, minP1, maxP1);

        start = Body::transform(QPointF(0, 0), b->getPos(), b->getAngle());
        QPointF end = Body::transform(fromVecToPoint(axcisList[i]), b->getPos(), b->getAngle());
        newAxcis = QVector2D(end - start);//Body::transform(axcisList[i], b->getPos(), b->getAngle());
        r->getProjection(newAxcis, start, minP2, maxP2);

        qDebug() << minP1 << maxP1 << minP2 << maxP2;
        if (minP1 >= minP2 && minP1 < maxP2){
            if (maxP2 - minP1 < penetration){
                penetration = maxP2 - minP1;
                normal = axcisList[i];
                qDebug() << "--------------------------";
            }
        }else if (maxP1 > minP2 && maxP1 <= maxP2){
            if (maxP1 - minP2 < penetration){
                penetration = maxP1 - minP2;
                normal = axcisList[i];
                qDebug() << "--------------------------";
            }
        }else{
            crossNum = 0;
            depth = 0;
            return;
        }
    }

    normal.normalize();
    depth = c->getRadius() - penetration;
    crossNum = 1;
    crossPoints[0] = fromVecToPoint(normal * c->getRadius() + QVector2D(a->getPos()));

}

void Collision::CircleWithPolygon(){
    //отримуємо круг та прямокутник
    Circle *c = reinterpret_cast<Circle *>(a->getShape());
    Rectangle *r = reinterpret_cast<Rectangle *>(b->getShape());

    //отримуємо список нормалей та вершин прямокутника
    QVector<QVector2D> axcisList = r->getNormals();
    QVector<QPointF> vertecies = r->getVertices();
    crossNum = 0;

    //трансформуємо координати центру кола в систему координат прямокутника
    QPointF center = a->getPos();
    center = fromVecToPoint(Mat2(r->getU().Transpose()) * QVector2D((center - b->getPos())));

    //знаходимо "лицьове" ребро
    //принцип схожий на використання опорних точок
    float separation = -9999999;
    int faceNormal = 0;
    for(int i = 0; i < axcisList.size(); i++){
        float s = QVector2D::dotProduct(axcisList[i], QVector2D(center - vertecies[i]));

        //якщо радіус меньший ніж відстань від центру до ребра,
        //то об'єкти не перетинаються
        if(s > c->getRadius())
            return;

        if(s > separation){
            separation = s;
            faceNormal = i;
        }
    }

    //знаходимо дві вершини інцидентні "лицьовому" ребру
    QPointF v1 = vertecies[faceNormal];
    int i2 = faceNormal + 1 < vertecies.size() ? faceNormal + 1 : 0;
    QPointF v2 = vertecies[i2];

    //перевіряємо чи знаходиться центр всередині прямокутника
    //якщо separation від'ємне або близьке до нуля, то центр всередині або на ребрі
    if(separation < 0.0001){
        crossNum = 1;
        normal = -(r->getU() * axcisList[faceNormal]);
        crossPoints[0] = fromVecToPoint(normal * c->getRadius() + QVector2D(a->getPos()));
        depth = c->getRadius();
        return;
    }

    //визначаємо у якій області знаходиться центр кола
    float dot1 = QVector2D::dotProduct(QVector2D(center - v1), QVector2D(v2 - v1));
    float dot2 = QVector2D::dotProduct(QVector2D(center - v2), QVector2D(v1 - v2));
    depth = c->getRadius() - separation;

    //центр кола ближче до вершини V1
    if(dot1 <= 0.0f){
        if(QVector2D(center - v1).lengthSquared() > c->getRadius() * c->getRadius())
            return;

        crossNum = 1;
        QVector2D n(v1 - center);
        n = r->getU() * n;
        n.normalize();
        normal = n;
        v1 = fromVecToPoint(Mat2(r->getU()) * QVector2D(v1) + QVector2D(b->getPos()));
        crossPoints[0] = v1;
    }

    //центр кола ближче до вершини V2
    else if(dot2 <= 0.0f){
        if(QVector2D(center - v2).lengthSquared() > c->getRadius() * c->getRadius())
            return;

        crossNum = 1;
        QVector2D n(v2 - center);
        v2 = fromVecToPoint(Mat2(r->getU()) * QVector2D(v2) + QVector2D(b->getPos()));
        crossPoints[0] = v2;
        n = r->getU() * n;
        n.normalize();
        normal = n;
    }

    //центр кола знаходиться між V1 та V2
    else{
        QVector2D n = axcisList[faceNormal];
        if(QVector2D::dotProduct(QVector2D(center - v1), n) > c->getRadius())
            return;

        n = r->getU() * n;
        normal = -n;
            crossPoints[0] = fromVecToPoint(normal * c->getRadius() + QVector2D(a->getPos()));
            crossNum = 1;
    }
}

void Collision::PolygonWithPolygon(){
    //отримуємо два многокутника
    Rectangle *r1  = reinterpret_cast<Rectangle *>(a->getShape());
    Rectangle *r2  = reinterpret_cast<Rectangle *>(b->getShape());

    crossNum = 0;

    //перевіряємо наявність розділової осі на основі нормалей r1
    //шукаємо "лицьове" ребро та глибину проникнення для r1
    int faceA;
    float penetrationA = FindAxisLeastPenetration( &faceA, r1, r2, a, b);
    if(penetrationA >= 0.0f)
      return;

    //перевіряємо наявність розділової осі на основі нормалей r2
    //шукаємо "лицьове" ребро та глибину проникнення для r2
    int faceB;
    float penetrationB = FindAxisLeastPenetration( &faceB, r2, r1, b, a);
    if(penetrationB >= 0.0f)
      return;

    int referenceIndex;
    bool flip; //змінна вказує чи напрямок від r1 до r2

    Rectangle *RefPoly; //референтний многокутник
    Rectangle *IncPoly; //інцидентний многокутник
    Body *RefPolyBody;
    Body *IncPolyBody;

    //визначаємо референтний та інцидентний многокутник
    if(BiasGreaterThan( penetrationA, penetrationB )){
      RefPoly = r1;
      IncPoly = r2;
      RefPolyBody = a;
      IncPolyBody = b;
      referenceIndex = faceA;
      flip = false;
    }
    else{
      RefPoly = r2;
      IncPoly = r1;
      RefPolyBody = b;
      IncPolyBody = a;
      referenceIndex = faceB;
      flip = true;
    }

    //шукаємо вершини інцидентного ребра у глобальних координатах
    QVector2D incidentFace[2];
    FindIncidentFace( incidentFace, RefPoly, IncPoly, RefPolyBody, IncPolyBody, RefPoly->getNormals()[referenceIndex]);

    //отримуємо дві вершини референтного многокутника
    QPointF v1 = RefPoly->getVertices()[referenceIndex];
    referenceIndex = referenceIndex + 1 == RefPoly->getVertices().size() ? 0 : referenceIndex + 1;
    QPointF v2 = RefPoly->getVertices()[referenceIndex];

    //трансформуємо їх до глобальної СК
    v1 = fromVecToPoint(RefPoly->getU() * QVector2D(v1) + QVector2D(RefPolyBody->getPos()));
    v2 = fromVecToPoint(RefPoly->getU() * QVector2D(v2) + QVector2D(RefPolyBody->getPos()));

    //направляючий вектор референтного ребра у глобальній СК
    QVector2D sidePlaneNormal = QVector2D(v2 - v1);
    sidePlaneNormal.normalize();

    //отримуємо референтну зовнішню нормаль шляхом повороту на 90 градусів
    QVector2D refFaceNormal(sidePlaneNormal.y(), -sidePlaneNormal.x());

    // ax + by = c
    // c - відстань до початку глобальної СК
    float refC = QVector2D::dotProduct(refFaceNormal, QVector2D(v1));
    float negSide = -QVector2D::dotProduct(sidePlaneNormal, QVector2D(v1));
    float posSide =  QVector2D::dotProduct(sidePlaneNormal, QVector2D(v2));

    //обрізаємо інцидентне ребро продовженнями референтних ребер
    if(Clip( -sidePlaneNormal, negSide, incidentFace ) < 2)
      return; //через похибку збереження даних у double точок можн не бути

    if(Clip(  sidePlaneNormal, posSide, incidentFace ) < 2)
      return; //через похибку збереження даних у double точок можн не бути

    //якщо потрібно, перевертаємо нормаль
    normal = flip ? -refFaceNormal : refFaceNormal;

    //збережемо точки, що знаходяться всередині многокутника
    int cp = 0; //лічільник точок
    float separation = QVector2D::dotProduct(refFaceNormal, incidentFace[0]) - refC;
    if(separation <= 0.0f){
      crossPoints[cp] = fromVecToPoint(incidentFace[0]);
      depth = -separation;
      ++cp;
    }
    else
      depth = 0;

    separation = QVector2D::dotProduct(refFaceNormal, incidentFace[1]) - refC;
    if(separation <= 0.0f){
      crossPoints[cp] = fromVecToPoint(incidentFace[1]);

      depth -= separation;
      cp++;

      //розраховуємо середнє проникнення
      depth /= (float)cp;
    }

    crossNum = cp;
}

float Collision::FindAxisLeastPenetration(int *faceIndex, Rectangle *A, Rectangle *B, Body* Ab, Body* Bb){
    float bestDistance = -99999999.f;
    int bestIndex;

    //отримуємо список нормалей та вершин многокутника
    QVector<QVector2D>norm = A->getNormals();
    QVector<QPointF>vert = A->getVertices();

    //для кожного ребра знаходимо відстань "заглиблення"
    for(int i = 0; i < vert.size(); i++){
        //отримуємо зовнішню нормаль
        QVector2D n = norm[i];
        //трансформуємо її у глобальну систему координат
        QVector2D nw = A->getU() * n;

        //трансформуємо її у систему координат другого многокутника
        Mat2 buT = B->getU().Transpose( );
        n = buT * nw;

        //шукаємо опорну точку у напрямку протилежному до зовнішньої нормалі
        QVector2D s(B->GetSupport(-n));

        //отримуємо вершину що належить ребру "і" та
        //трансформуємо її у систему координат многокутника B
        QVector2D v(vert[i]);
        v = A->getU() * v + QVector2D(Ab->getPos());
        v -= QVector2D(Bb->getPos());
        v = buT * v;

        //обчислюємо "знакову" відстань між вершинами у напрямку нормалі
        float d = QVector2D::dotProduct(n, s - v);

        //зберігаємо найбільший результат
        if(d > bestDistance)
        {
          bestDistance = d;
          bestIndex = i;
    }
  }

  *faceIndex = bestIndex;
  return bestDistance;
}

void Collision::FindIncidentFace(QVector2D *v, Rectangle *RefPoly, Rectangle *IncPoly, Body* Ab, Body* Bb, QVector2D referenceNormal){
    //отримуємо нормалі та вершини інцидентного многокутника
    QVector<QVector2D>norm = IncPoly->getNormals();
    QVector<QPointF>vert = IncPoly->getVertices();

    //трансформуємо нормаль reference до системи координат incident
    referenceNormal = RefPoly->getU() * referenceNormal; //до глобальної СК
    referenceNormal = IncPoly->getU().Transpose( ) * referenceNormal; //до СК incident

    //шукаємо найвіддаленіше у протилежному до нормалі напрямку ребро многокутника incident
    int incidentFace = 0;
    float minDot = 999999999.f;
    for(int i = 0; i < vert.size(); ++i){
      float dot = QVector2D::dotProduct(referenceNormal, norm[i]);
      if(dot < minDot){
        minDot = dot;
        incidentFace = i;
      }
    }

    //зберігаємо дві вершини знайденого ребра в глобальній СК
    v[0] = IncPoly->getU() * QVector2D(vert[incidentFace]) + QVector2D(Bb->getPos());
    incidentFace = incidentFace + 1 >= (int)vert.size() ? 0 : incidentFace + 1;
    v[1] = IncPoly->getU() * QVector2D(vert[incidentFace]) + QVector2D(Bb->getPos());
}

int Collision::Clip(QVector2D n, float c, QVector2D *face){
    int sp = 0;
    QVector2D out[2] = {
        face[0],
        face[1]
    };

    //отримуємо відстані від кожної кінцевої точки до лінії
    // d = ax + by - c
    float d1 = QVector2D::dotProduct(n, face[0]) - c;
    float d2 = QVector2D::dotProduct(n, face[1]) - c;

    //якщо від'ємна відстань, то точка не в межах многокутника. Обрізаємо
    if(d1 <= 0.0f) out[sp++] = face[0];
    if(d2 <= 0.0f) out[sp++] = face[1];

    //якщо точки по різні стороні від ребра
    if(d1 * d2 < 0.0f){
        //додаємо точку перетину із ребром
        float alpha = d1 / (d1 - d2);
        out[sp] = face[0] + alpha * (face[1] - face[0]);
        sp++;
    }

    //зберігаємо знайдені значення
    face[0] = out[0];
    face[1] = out[1];

    assert( sp != 3 );

    return sp;
}


void Collision::collisionManage(){
    int t1 = a->getShape()->getType();
    int t2 = b->getShape()->getType();

    if (t1 == Shape::CIRCLE){
        if (t2 == Shape::CIRCLE){
            CircleWithCircle();
        }else if (t2 == Shape::RECT){
            CircleWithPolygon();
        }
    }else if (t1 == Shape::RECT){
        if (t2 == Shape::CIRCLE){
            Body* t = a;
            a = b;
            b = t;
            CircleWithPolygon();
        }else if (t2 == Shape::RECT){
            PolygonWithPolygon();
        }
    }
    //Dispatch[A->shape->GetType( )][B->shape->GetType( )]( this, A, B );
}

QPointF Collision::fromVecToPoint(QVector2D v){
    return QPointF(v.x(), v.y());
}

QVector2D Collision::fromPointToVec(QPointF p){
    return QVector2D(p.x(), p.y());
}

float Collision::crossProduct(const QVector2D a, const QVector2D b){
    return a.x() * b.y() - a.y() * b.x();
}

QVector2D Collision::crossProduct(const QVector2D a, float s){
    return QVector2D(s * a.y(), -s * a.x());
}

QVector2D Collision::crossProduct(float s, const QVector2D a){
    return QVector2D(-s * a.y(), s * a.x());
}

