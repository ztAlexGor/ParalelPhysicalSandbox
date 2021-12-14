#include "collision.h"

Collision::Collision(Body* a, Body* b){
    this->a = a;
    this->b = b;
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

    // Calculate static and dynamic friction
    sf = sqrt(a->getSFriction() * a->getSFriction()); //todo
    df = sqrt(a->getDFriction() * a->getDFriction());

    for(int i = 0; i < crossNum; i++){
        QVector2D ra = fromPointToVec(crossPoints[i] - a->getPos());
        QVector2D rb = fromPointToVec(crossPoints[i] - b->getPos());

        QVector2D rv = (b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
                        a->getVelocity() - crossProduct(a->getAngularVel(), ra));

        // Determine if we should perform a resting collision or not
        // The idea is if the only thing moving this object is gravity,
        // then the collision should be performed without any restitution
        if(rv.lengthSquared() < (time * a->gravity).lengthSquared( ) + 0.0001)
            minResilience = 0.0; // todo
    }
}

void Collision::fixCollision(){

    if(a->isStatic() && b->isStatic()){
//        InfiniteMassCorrection( );
        return;
    }

    for(int i = 0; i < crossNum; i++){
        QVector2D ra = fromPointToVec(crossPoints[i] - a->getPos());
        QVector2D rb = fromPointToVec(crossPoints[i] - b->getPos());

        QVector2D rv = (b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
                        a->getVelocity() - crossProduct(a->getAngularVel(), ra));


        // Relative velocity along the normal
        float contactVel = QVector2D::dotProduct(rv, normal);

        // Do not resolve if velocities are separating
        if(contactVel > 0)
            return;

        float raCrossN = crossProduct(ra, normal);
        float rbCrossN = crossProduct(rb, normal);
        float invMassSum = a->getInvMass() + b->getInvMass() + raCrossN * raCrossN * a->getInvInertia()
                                                              + rbCrossN * rbCrossN * b->getInvInertia();

        // Calculate impulse scalar
        float j = -(1.0f + minResilience) * contactVel;
        j /= invMassSum;
        j /= (float)crossNum;

        // Apply impulse
        QVector2D impulse = normal * j;
        a->applyImpulse(-impulse, ra);
        b->applyImpulse(impulse, rb);

        // Friction impulse
        rv = b->getVelocity() + crossProduct(b->getAngularVel(), rb) -
             a->getVelocity() - crossProduct(a->getAngularVel(), ra);

        QVector2D t = rv - (normal * QVector2D::dotProduct(rv, normal));
        t.normalize();

        // j tangent magnitude
        float jt = - QVector2D::dotProduct(rv, t);
        jt /= invMassSum;
        jt /= (float)crossNum;

        // Don't apply tiny friction impulses
        if(jt < 0.0001)return;

        // Coulumb's law
        QVector2D tangentImpulse;
        if(abs(jt) < j * sf)
          tangentImpulse = t * jt;
        else
          tangentImpulse = t * (-j) * df;

        // Apply friction impulse
        a->applyImpulse(-tangentImpulse, ra);
        b->applyImpulse(tangentImpulse, rb);
  }
}

void Collision::positionalCorrection(){
  const float k_slop = 0.05f; // Penetration allowance
  const float percent = 0.4f; // Penetration percentage to correct

  QVector2D correction = QVector2D(0, 0);

  float t = depth - k_slop;
  if (t > 0){
      correction = (t / (a->getInvMass() + b->getInvMass())) * normal * percent;
  }

  a->setPos(fromVecToPoint(QVector2D(a->getPos()) - correction * a->getInvMass()));
  b->setPos(fromVecToPoint(QVector2D(b->getPos()) + correction * b->getInvMass()));
}

void Collision::CircleWithCircle(){
    Circle *c1  = reinterpret_cast<Circle *>(a->getShape());
    Circle *c2  = reinterpret_cast<Circle *>(b->getShape());

    normal = QVector2D(b->getPos() - a->getPos());
    float dist = normal.lengthSquared();
    float sumOfRadiuses = c1->getRadius() + c2->getRadius();

    if (dist >= sumOfRadiuses * sumOfRadiuses){
        crossNum = 0;
    }else if (dist == 0){
        crossNum = 1;
        normal = QVector2D(1, 0);
        depth = c1->getRadius();
        crossPoints[0] = a->getPos();
    }else{
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
    float penetration = 999999999;

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
    Circle *c = reinterpret_cast<Circle *>(a->getShape());
    Rectangle *r = reinterpret_cast<Rectangle *>(b->getShape());

    QVector<QVector2D> axcisList = r->getNormals();
    QVector<QPointF> vertecies = r->getVertices();
    crossNum = 0;

    // Transform circle center to Polygon model space
    QPointF center = a->getPos();
    center = fromVecToPoint(Mat2(r->getU().Transpose()) * QVector2D((center - b->getPos())));

    // Find edge with minimum penetration
    // Exact concept as using support points in Polygon vs Polygon
    float separation = -FLT_MAX;
    int faceNormal = 0;
    for(int i = 0; i < axcisList.size(); i++){
        float s = QVector2D::dotProduct(axcisList[i], QVector2D(center - vertecies[i]));

        if(s > c->getRadius())
            return;

        if(s > separation){
            separation = s;
            faceNormal = i;
        }
    }

    // Grab face's vertices
    QPointF v1 = vertecies[faceNormal];
    int i2 = faceNormal + 1 < vertecies.size() ? faceNormal + 1 : 0;
    QPointF v2 = vertecies[i2];

    // Check to see if center is within polygon
    if(separation < 0.0001)
    {
      crossNum = 1;
      normal = -(r->getU() * axcisList[faceNormal]);
      crossPoints[0] = fromVecToPoint(normal * c->getRadius() + QVector2D(a->getPos()));
      depth = c->getRadius();
      return;
    }

    // Determine which voronoi region of the edge center of circle lies within
    float dot1 = QVector2D::dotProduct(QVector2D(center - v1), QVector2D(v2 - v1));
    float dot2 = QVector2D::dotProduct(QVector2D(center - v2), QVector2D(v1 - v2));
    depth = c->getRadius() - separation;

    // Closest to v1
    if(dot1 <= 0.0f)
    {
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

    // Closest to v2
    else if(dot2 <= 0.0f)
    {
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

    // Closest to face
    else
    {
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
    Rectangle *r1  = reinterpret_cast<Rectangle *>(a->getShape());
    Rectangle *r2  = reinterpret_cast<Rectangle *>(b->getShape());

    crossNum = 0;

    // Check for a separating axis with A's face planes
    int faceA;
    float penetrationA = FindAxisLeastPenetration( &faceA, r1, r2, a, b);
    if(penetrationA >= 0.0f)
      return;

    // Check for a separating axis with B's face planes
    int faceB;
    float penetrationB = FindAxisLeastPenetration( &faceB, r2, r1, b, a);
    if(penetrationB >= 0.0f)
      return;

    int referenceIndex;
    bool flip; // Always point from a to b

    Rectangle *RefPoly; // Reference
    Rectangle *IncPoly; // Incident
    Body *RefPolyBody;
    Body *IncPolyBody;
    // Determine which shape contains reference face
    if(BiasGreaterThan( penetrationA, penetrationB ))
    {
      RefPoly = r1;
      IncPoly = r2;
      RefPolyBody = a;
      IncPolyBody = b;
      referenceIndex = faceA;
      flip = false;
    }

    else
    {
      RefPoly = r2;
      IncPoly = r1;
      RefPolyBody = b;
      IncPolyBody = a;
      referenceIndex = faceB;
      flip = true;
    }

    // World space incident face
    QVector2D incidentFace[2];
    FindIncidentFace( incidentFace, RefPoly, IncPoly, RefPolyBody, IncPolyBody, RefPoly->getNormals()[referenceIndex]);

    //        y
    //        ^  ->n       ^
    //      +---c ------posPlane--
    //  x < | i |\
    //      +---+ c-----negPlane--
    //             \       v
    //              r
    //
    //  r : reference face
    //  i : incident poly
    //  c : clipped point
    //  n : incident normal

    // Setup reference face vertices


    QPointF v1 = RefPoly->getVertices()[referenceIndex];
    referenceIndex = referenceIndex + 1 == RefPoly->getVertices().size() ? 0 : referenceIndex + 1;
    QPointF v2 = RefPoly->getVertices()[referenceIndex];

    // Transform vertices to world space
    v1 = fromVecToPoint(RefPoly->getU() * QVector2D(v1) + QVector2D(RefPolyBody->getPos()));
    v2 = fromVecToPoint(RefPoly->getU() * QVector2D(v2) + QVector2D(RefPolyBody->getPos()));

    // Calculate reference face side normal in world space
    QVector2D sidePlaneNormal = QVector2D(v2 - v1);
    sidePlaneNormal.normalize();

    // Orthogonalize
    QVector2D refFaceNormal(sidePlaneNormal.y(), -sidePlaneNormal.x());

    // ax + by = c
    // c is distance from origin
    float refC = QVector2D::dotProduct(refFaceNormal, QVector2D(v1));
    float negSide = -QVector2D::dotProduct(sidePlaneNormal, QVector2D(v1));
    float posSide =  QVector2D::dotProduct(sidePlaneNormal, QVector2D(v2));

    // Clip incident face to reference face side planes
    if(Clip( -sidePlaneNormal, negSide, incidentFace ) < 2)
      return; // Due to floating point error, possible to not have required points

    if(Clip(  sidePlaneNormal, posSide, incidentFace ) < 2)
      return; // Due to floating point error, possible to not have required points

    // Flip
    normal = flip ? -refFaceNormal : refFaceNormal;

    // Keep points behind reference face
    int cp = 0; // clipped points behind reference face
    float separation = QVector2D::dotProduct(refFaceNormal, incidentFace[0]) - refC;
    if(separation <= 0.0f)
    {
      crossPoints[cp] = fromVecToPoint(incidentFace[0]);
      depth = -separation;
      ++cp;
    }
    else
      depth = 0;

    separation = QVector2D::dotProduct(refFaceNormal, incidentFace[1]) - refC;
    if(separation <= 0.0f)
    {
      crossPoints[cp] = fromVecToPoint(incidentFace[1]);

      depth += -separation;
      ++cp;

      // Average penetration
      depth /= (float)cp;
    }

    crossNum = cp;
}

float Collision::FindAxisLeastPenetration(int *faceIndex, Rectangle *A, Rectangle *B, Body* Ab, Body* Bb){
    float bestDistance = -FLT_MAX;
    int bestIndex;
    QVector<QVector2D>norm = A->getNormals();
    QVector<QPointF>vert = A->getVertices();

    for(int i = 0; i < vert.size(); i++){
        // Retrieve a face normal from A
        QVector2D n = norm[i];
        QVector2D nw = A->getU() * n;

        // Transform face normal into B's model space
        Mat2 buT = B->getU().Transpose( );
        n = buT * nw;

        // Retrieve support point from B along -n
        QVector2D s(B->GetSupport(-n));

        // Retrieve vertex on face from A, transform into
        // B's model space
        QVector2D v(vert[i]);
        v = A->getU() * v + QVector2D(Ab->getPos());
        v -= QVector2D(Bb->getPos());
        v = buT * v;

        // Compute penetration distance (in B's model space)
        float d = QVector2D::dotProduct(n, s - v);

        // Store greatest distance
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
    QVector<QVector2D>norm = IncPoly->getNormals();
    QVector<QPointF>vert = IncPoly->getVertices();

  // Calculate normal in incident's frame of reference
  referenceNormal = RefPoly->getU() * referenceNormal; // To world space
  referenceNormal = IncPoly->getU().Transpose( ) * referenceNormal; // To incident's model space

  // Find most anti-normal face on incident polygon
  int incidentFace = 0;
  float minDot = FLT_MAX;
  for(int i = 0; i < vert.size(); ++i){
    float dot = QVector2D::dotProduct(referenceNormal, norm[i]);
    if(dot < minDot){
      minDot = dot;
      incidentFace = i;
    }
  }

  // Assign face vertices for incidentFace
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

  // Retrieve distances from each endpoint to the line
  // d = ax + by - c
  float d1 = QVector2D::dotProduct(n, face[0]) - c;
  float d2 = QVector2D::dotProduct(n, face[1]) - c;

  // If negative (behind plane) clip
  if(d1 <= 0.0f) out[sp++] = face[0];
  if(d2 <= 0.0f) out[sp++] = face[1];

  // If the points are on different sides of the plane
  if(d1 * d2 < 0.0f) // less than to ignore -0.0f
  {
    // Push interesection point
    float alpha = d1 / (d1 - d2);
    out[sp] = face[0] + alpha * (face[1] - face[0]);
    ++sp;
  }

  // Assign our new converted values
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







