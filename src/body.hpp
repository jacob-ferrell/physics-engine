#pragma once

#include <raylib.h>
#include <cmath>
#include "vec2.hpp"
#include "shape.hpp"
#include "contact.hpp"

class Body {
    Vec2 position_;
    Vec2 velocity_ = {0, 0};
    Shape shape_;
    int id_;
    float restitution_ = 0.8f;
    float mass_;
    float inertia_;
    float inverseMass_;
    float inverseInertia_;
    float density_;
    float angle_ = (float)M_PI / 2.0f;
    float angularVelocity_ = 0.0;
    float friction_;

public:

    Body(int id, float radius, Vec2 position, float density = 1.0f, float friction = 0.4f)
        :  position_(position), shape_(Circle{radius}), id_(id), 
           density_{density}, friction_(friction) {
        mass_ = area() * density_;
        inverseMass_ = (mass_ > 0.0f) ? 1.0f / mass_ : 0.0f;
        inertia_ = calcInertia();
        inverseInertia_ = calcInverseInertia();
    }

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
    float inverseMass() const { return inverseMass_; };
    static Body makeCircle(const int nextBodyId, const float radius, const float x, const float y);
    static Body makeCircle(const int nextBodyId, const float radius, const float x, const float y, const float density);
    void handleCircleCollision(Body& b, const Contact& contact);
    float area() const; 
    void drag(const Vec2& distance, const float& dt); 
    void draw() const;
    bool overlaps(const Body& b) const;
    float inertia() const { return inertia_; };
    float inverseInertia() const { return inverseInertia_; };

private:
    float calcInertia() const;
    float calcInverseInertia() const;
    float mass() const { return mass_;}
    float collisionRestitution(const Body& other) const { return std::min(restitution_, other.restitution_); }
    float collisionFriction(const Body& other) const { return std::sqrt(friction_ * other.friction_); }
    //void applyImpulse(Body& other, const Vec2& normal);
    void adjustVelocity(Body& other, const Contact& contact);
    void correct(Body& other, const Vec2& correction);
    void applyFrictionImpulse(Body& other, const Contact& contact, const float& impulseMagnitute);
};










