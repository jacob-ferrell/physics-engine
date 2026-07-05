#include "body.hpp"
#include "vec2.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>

Vec2 halfExtents(const Body& body) {
    if (const auto* c = std::get_if<Circle>(&body.shape)) {
        return {c->radius, c->radius};
    } else if (const auto* box = std::get_if<Box>(&body.shape)) {
        return {box->width / 2, box->height / 2};
    }
    assert(false && "unhandled Shape type");

    return {0, 0};

}

void bounceBody(Vec2& c, Body& body) {
    body.state.position += c;
    float hitX = c.x != 0;
    float hitY = c.y != 0;

    float eX = std::abs(body.state.velocity.x) > RESTITUTION_THRESHOLD ? body.restitution : 0.0f;
    float eY = std::abs(body.state.velocity.y) > RESTITUTION_THRESHOLD ? body.restitution : 0.0f;

    body.state.velocity.x *= 1.0f - hitX * (1.0f + eX);
    body.state.velocity.y *= 1.0f - hitY * (1.0f + eY);

}


Body makeCircle(float radius, float x, float y) {
    Circle c = {
        .radius = radius
    };

    MotionState state = MotionState{
        .position = Vec2{x, y}
    };

    Body body = {
        .state = state,
        .shape = c
    };

    return body;
    
}

void handleCircleCollision(Body& a, Body& b, const float& radiusSum) {

    Vec2 direction = b.state.position - a.state.position;
    float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));

    if (length == 0.0f) return; //avoid dividing by 0

    Vec2 normal = direction / length;

    Vec2 relVelocity = b.state.velocity - a.state.velocity;
    float velAlongNormal = dotProduct(relVelocity, normal);

    if (velAlongNormal > 0) return; // already moving apart

    float invMassA = 1, invMassB = 1; // placeholders for when mass is introduced

    float e = std::min(a.restitution, b.restitution);

    float impulseMagnitute = -(1 + e) * velAlongNormal / (invMassA + invMassB);

    a.state.velocity -= impulseMagnitute * invMassA * normal;
    b.state.velocity += impulseMagnitute * invMassB * normal;

    float penetration = radiusSum - length;

    Vec2 correction = normal * penetration;

    a.state.position -= correction * (invMassA / (invMassA + invMassB));
    b.state.position += correction * (invMassB / (invMassA + invMassB));

}

void handleCollision(Body& a, Body& b) {
    const auto* cA = std::get_if<Circle>(&a.shape);
    const auto* cB = std::get_if<Circle>(&b.shape);

    if (cA && cB) {
        Vec2 d = a.state.position - b.state.position;
        float dSq = d.x * d.x + d.y * d.y;
        float radiusSum = cA->radius + cB->radius;
        if (dSq < radiusSum * radiusSum) {
            handleCircleCollision(a, b, radiusSum);
        }
    }

}


