#pragma once 

#include <cstdlib>
struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;
};

inline Vec2 operator+(const Vec2& a, const Vec2& b) {
     return {a.x + b.x, a.y + b.y};
}

inline void operator+=(Vec2& a, const Vec2& b) {
    a.x += b.x;
    a.y += b.y;
}

inline void operator+=(Vec2& a, const float& f) {
    a.x += f;
    a.y += f;
}

inline Vec2 operator-(const Vec2& a, const Vec2& b) {
     return {a.x - b.x, a.y - b.y};
}

inline void operator-=(Vec2& a, const Vec2& b) {
    a.x -= b.x;
    a.y -= b.y;
}

inline void operator-=(Vec2& a, const float& f) {
    a.x -= f;
    a.y -= f;
}

inline void operator/=(Vec2& a, const float& f) {
    a.x /= f;
    a.y /= f;
}

inline Vec2 operator*(const Vec2 &a, const Vec2 &b) {
    return {a.x * b.x, a.y * b.y};
}

inline Vec2 operator*(const Vec2& a, const float& f) {
    return {a.x * f, a.y * f};
}

inline Vec2 operator*(const float& f, const Vec2& a) {
    return {a.x * f, a.y * f};
}

inline float dotProduct(const Vec2 &a, const Vec2 &b) {
    return a.x * b.x + a.y * b.y;
}

inline float crossProduct(const Vec2 &a, const Vec2 &b) {
    return a.x * b.y - b.x * a.y;
}

inline Vec2 abs(const Vec2& a) {
    return Vec2{std::abs(a.x), std::abs(a.y)};
}

inline Vec2 operator/(const Vec2& a, float b) {
    return Vec2{
        a.x / b,
        a.y / b
    };
}

inline Vec2 perpendicular(const Vec2& a) {
    return {-a.y, a.x};
}

    
Vec2 calcNormal(const Vec2& a, const Vec2& b);
