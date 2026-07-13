#pragma once

#include <raylib.h>
#include "vec2.hpp"
#include "shape.hpp"

class Body {
    Vec2 position_;
    Vec2 velocity_ = {0, 0};
    Shape shape_;
    int id_;
    float restitution_ = 0.8f;
    float density_ = 1.0f;

public:
    Body(int id, float radius, Vec2 position, float density)
        :  position_(position), shape_(Circle{radius}), id_(id), density_{density} {}

    Body(int id, float radius, Vec2 position)
        :  position_(position), shape_(Circle{radius}), id_(id) {}
    

    const int& id() const { return id_; }
    const Shape& shape() const { return shape_; }
    const Vec2& position() const { return position_; }

    Vec2 halfExtents() const; 
    void integrate(const float& dt, const Vec2& gravity);
    void bounce(Vec2& c); 
    void handleCollision(Body& other);
    bool containsPosition(const Vec2& position) const;
    bool containsPosition(const Vector2& position) const;
    void freeze();
    float inverseMass() const;
    static Body makeCircle(const int nextBodyId, const float radius, const float x, const float y);
    static Body makeCircle(const int nextBodyId, const float radius, const float x, const float y, const float density);
    void handleCircleCollision(Body& b, const float& radiusSum);
    float area() const; 
    void drag(const Vec2& distance, const float& dt); 
    void draw() const;
    bool overlaps(const Body& b) const;

};










