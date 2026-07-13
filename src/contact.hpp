#pragma once

#include <optional>
#include "vec2.hpp"

class Body;

struct Contact {
    Vec2 normal;
    Vec2 rA; // Body a.center -> contact point
    Vec2 rB; // Body b.center -> contact point
    Vec2 correction;
    float penetration;
};

std::optional<Contact> circleCircle(const Body& cA, const Body& cB, const float& radiusA, const float& radiusB);
