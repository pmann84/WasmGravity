#include "vector.hpp"

Vector2::Vector2(float x, float y) : m_x(x), m_y(y)
{
}

float Vector2::getX() const
{
    return m_x;
}

float Vector2::getY() const
{
    return m_y;
}

void Vector2::setX(float val)
{
    m_x = val;
}

void Vector2::setY(float val)
{
    m_y = val;
}
