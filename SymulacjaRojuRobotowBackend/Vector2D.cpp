#include "pch.h"
#include "Vector2D.h"
#include <cmath>

using namespace std;

        Vector2D::Vector2D()
            {
                this -> x = 0;
                this -> y = 0;
            }

        Vector2D::Vector2D(double x, double y)
            {
                this -> x = x;
                this -> y = y;
            }

        Vector2D::Vector2D(const Vector2D& c)
            {
                this -> x = c.x;
                this -> y = c.y;
            }

        Vector2D Vector2D::operator+(const Vector2D& other) const
            {
                return Vector2D(x + other.x, y + other.y);
            }

         Vector2D Vector2D::operator-(const Vector2D& other) const
            {
                return Vector2D(x - other.x, y - other.y);
            }

        Vector2D Vector2D::operator*(double scalar) const
            {
                return Vector2D(x * scalar, y * scalar);
            }

        Vector2D Vector2D::operator/(double scalar) const
            {
                return Vector2D(x / scalar, y / scalar);
            }

        double Vector2D::mag()
            {
                return sqrt(x*x + y*y);
            }