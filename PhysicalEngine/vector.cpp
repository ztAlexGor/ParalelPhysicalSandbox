#include "vector.h"

Vector::Vector(): X(0), Y(0){}

Vector::Vector(double x, double y): X(x), Y(y){}

Vector::Vector(const Vector& other){
    this->X = other.X;
    this->Y = other.Y;
}

Vector& Vector::operator=(const Vector& other){
    if (this == &other) return *this;

    this->X = other.X;
    this->Y = other.Y;
    return *this;
}

bool Vector::isGreater(const Vector& other) const {
    if (this->X > other.X && this->Y > other.Y) return true;
    else return false;
}
