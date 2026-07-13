#include "contact.hpp"
#include <cmath>
#include <optional>
#include "body.hpp"


std::optional<Contact> circleCircle(const Body& cA, const Body& cB, const float& radiusA, const float& radiusB) {
    Vec2 d = cA.position() - cB.position();

    float dSq = d.x * d.x + d.y * d.y;

    float radiusSum = radiusA + radiusB;

    if (dSq >= radiusSum * radiusSum) {
        return std::nullopt;
    }

    Vec2 direction = cB.position() - cA.position();

    float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));

    if (length == 0.0f) return std::nullopt; //avoid dividing by 0

    Vec2 normal = direction / length;

    float penetration = radiusSum - length;

    Vec2 rA = normal * radiusA;

    Vec2 rB = (-1.0f * normal) * radiusB;

    return Contact{
        .normal = normal,
        .rA = rA,
        .rB = rB,
        .correction = normal * penetration,
        .penetration = penetration
    };

}
