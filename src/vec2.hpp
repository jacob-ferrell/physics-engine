#pragma once 

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) {
     return {a.x + b.x, a.y + b.y};
}

inline Vec2 operator-(const Vec2& a, const Vec2& b) {
     return {a.x - b.x, a.y - b.y};
}

inline Vec2 operator*(const Vec2& a, const float& f) {
    return {a.x * f, a.y * f};
}

inline void operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x;
    a.y += b.y;
}


