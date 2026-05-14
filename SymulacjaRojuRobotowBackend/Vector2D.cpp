#include "pch.h"
#include "Vector2D.h"
#include <cmath>

using namespace std;

        Vector2D::Vector2D()
            {
                this -> x = 0.0f;
                this -> y = 0.0f;
            }

        Vector2D::Vector2D(float x, float y, float maxMag)
            {
                this -> x = x;
                this -> y = y;
                this->maxMag = maxMag;
            }
        Vector2D::Vector2D(float x, float y)
        {
            this->x = x;
            this->y = y;
        }

        Vector2D::Vector2D(const Vector2D& c)
            {
                this -> x = c.x;
                this -> y = c.y;
                this->maxMag = c.maxMag;
            }

        Vector2D Vector2D::operator+(const Vector2D& other) const
            {
                return Vector2D(x + other.x, y + other.y);
            }

         Vector2D Vector2D::operator-(const Vector2D& other) const
            {
                return Vector2D(x - other.x, y - other.y);
            }

        Vector2D Vector2D::operator*(float scalar) const
            {
                return Vector2D(x * scalar, y * scalar);
            }

        float Vector2D::mag() const
            {
                return sqrt(x*x + y*y);
            }

        Vector2D Vector2D::normalized() const
            {
                float m = mag();
                if (m > 0) return Vector2D(x / m, y / m);
                else return Vector2D(0, 0);
            }

        void Vector2D::limit(float maxMag)
            {
                if (mag() > 0)
                    {
                        *this = normalized() * maxMag; // Change current object to maxMag
                    }
            }

        float Vector2D::distanceTo(const Vector2D& other) const
            {
                return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
            }