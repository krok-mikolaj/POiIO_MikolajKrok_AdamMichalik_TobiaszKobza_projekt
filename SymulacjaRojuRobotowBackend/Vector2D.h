#pragma once

class Vector2D{
    public:
        float x, y;

        Vector2D();
        Vector2D(float x, float y);
        Vector2D(const Vector2D& c); // copy constructor
        ~Vector2D(); // destructor

        Vector2D operator+(const Vector2D& other) const;
        Vector2D operator-(const Vector2D& other) const;
        Vector2D operator*(float scalar) const;

        float mag() const;
        Vector2D normalized() const;
        void limit(float maxMag);
        float distanceTo(const Vector2D& other) const;
};