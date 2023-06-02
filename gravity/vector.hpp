#pragma once

#include <emscripten/bind.h>

class Vector2
{
public:
    Vector2(float x, float y);

    float getX() const;
    void setX(float val);
    float getY() const;
    void setY(float val);

private:
    float m_x, m_y;
};

EMSCRIPTEN_BINDINGS(Gravity) {
    emscripten::class_<Vector2>("Vector2")
                .constructor<float, float>()
                .property("x", &Vector2::getX, &Vector2::setX)
                .property("y", &Vector2::getY, &Vector2::setY);
}