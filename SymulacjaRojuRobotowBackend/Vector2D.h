#pragma once

class Vector2D{
    private:
        double x,y;

    public:
        Vector2D();
        Vector2D(double x, double y);
        Vector2D(const Vector2D& c); // copy constructor
        ~Vector2D(); // destructor

        Vector2D operator+(const Vector2D& other) const;
        Vector2D operator-(const Vector2D& other) const;
        Vector2D operator*(double scalar) const;
        Vector2D operator/(double scalar) const;

        double mag();

};