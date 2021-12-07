#ifndef VECTOR_H
#define VECTOR_H


class Vector{
public:
    double X;
    double Y;
    Vector();
    Vector(double, double);
    Vector(const Vector&);
    Vector& operator=(const Vector& other);
    bool isGreater(const Vector&)const;
};

#endif // VECTOR_H
