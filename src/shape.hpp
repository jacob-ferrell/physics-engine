#pragma once 

#include <variant>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2 operator*(Vec2 a, Vec2 b) {
     return {a.x * b.x, a.y * b.y};
}

inline Vec2 operator+(Vec2 a, Vec2 b) {
     return {a.x + b.x, a.y + b.y};
}

inline Vec2 operator*(Vec2 a, float f) {
    return {a.x * f, a.y * f};
}

inline Vec2 operator+=(Vec2 a, Vec2 b) {
    return {a.x + b.x, a.y +b.y};
}

struct MotionState {
    Vec2 position;
    Vec2 velocity{0, 0};
};

struct Circle {
    float radius;
};

struct Box {
    float width;
    float height;
};

using Shape = std::variant<Circle, Box>;

struct Body {
    MotionState state;
    Shape shape;
};

Vec2 halfExtents(Body& body); 

void updatePosition(const float& dt, Body& body);
