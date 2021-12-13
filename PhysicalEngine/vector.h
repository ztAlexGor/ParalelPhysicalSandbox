#ifndef VECTOR_H
#define VECTOR_H


class Vector{
public:
    float X;
    float Y;
    Vector();
    Vector(float, float);
    Vector(const Vector&);
    Vector& operator=(const Vector& other);
    Vector operator+(const Vector& other);
    Vector& operator+=(const Vector& other);
    bool isGreater(const Vector&)const;
};

#endif // VECTOR_H
