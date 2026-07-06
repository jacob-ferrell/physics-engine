#pragma once

#include <raylib.h>
#include <optional>
#include "vec2.hpp"
#include "shape.hpp"

struct Grab {
    Vec2 lastMousePosition;
};

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
    float density = 1.0f;
    std::optional<Grab> grab = std::nullopt;
};

Vec2 halfExtents(const Body& body); 

void integrate(const float& dt, MotionState& state);

void bounceBody(Vec2& c, Body& body); 

Body makeCircle(float radius, float x, float y);
Body makeCircle(float radius, float x, float y, float density);

void handleCircleCollision(Body& a, Body& b, const float& radiusSum);

void handleCollision(Body& a, Body& b);

// if given position exists within the body
bool positionWithinBody(const Body& body, const Vec2& position);
bool positionWithinBody(const Body& body, const Vector2& position);

// 'Grabs' a body when clicked on by a user.
void grab(Body& body, const Vec2& mousePostion);

float inverseMass(const Body& body);





