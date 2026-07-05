#include "vec2.hpp"
#include <cmath>

Vec2 calcNormal(const Vec2& a, const Vec2& b) {
    Vec2 direction = b - a;
    float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));
    return direction / length;
}
