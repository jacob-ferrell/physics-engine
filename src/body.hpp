#pragma once

#include "vec2.hpp"
#include "shape.hpp"

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

