#pragma once

#include "vec2.hpp"
#include "shape.hpp"

struct BoundsContact {
    Vec2 correction{0, 0};
};

struct MotionState {
    Vec2 position;
    Vec2 velocity{0, 0};
};

struct Body {
    MotionState state;
    Shape shape;
    float restitution = 0.8f;
};

Vec2 halfExtents(const Body& body); 

void integrate(const float& dt, MotionState& state);

void bounceBody(Vec2& c, Body& body); 

Body makeCircle(float radius, float x, float y);

void handleCircleCollision(Body& a, Body& b, const float& radiusSum);

void handleCollision(Body& a, Body& b);




