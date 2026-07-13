#include "body.hpp"
#include <raylib.h>
#include "shape.hpp"
#include "vec2.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>

void Body::freeze() {
    velocity_.x = 0.0f;
    velocity_.y = 0.0f;
}

float Body::area() const {

    if (auto* c = std::get_if<Circle>(&shape_)) {
        return (float)M_PI * c->radius * c->radius;
    }

    if (auto* b = std::get_if<Box>(&shape_)) {
        return b->width * b->height;
    }

    assert(false && "unhandled shape");

    return 0.0f;
 
}

float Body::inverseMass() const {
    float a = area();
    float mass = a * density_;
    return (mass > 0.0f) ? 1.0f / mass : 0.0f;
}

bool Body::containsPosition(const Vec2& position) const {
    Vec2 bodyPosition = position_;
    Vec2 absDistance = abs(bodyPosition - position);
    Vec2 extents = halfExtents();
    return absDistance.x <= extents.x && absDistance.y <= extents.y; 
}

bool Body::containsPosition(const Vector2& position) const {
    return containsPosition(Vec2{position.x, position.y});
}

Vec2 Body::halfExtents() const {
    if (const auto* c = std::get_if<Circle>(&shape_)) {
        return {c->radius, c->radius};
    } else if (const auto* box = std::get_if<Box>(&shape_)) {
        return {box->width / 2, box->height / 2};
    }
    assert(false && "unhandled Shape type");

    return {0, 0};

}

void Body::integrate(const float& dt, const Vec2& gravity) {
    velocity_ = velocity_ + gravity * dt; // apply gravity
    position_ += velocity_ * dt;
}

void Body::bounce(Vec2& c) {
    position_ += c;
    float hitX = c.x != 0;
    float hitY = c.y != 0;

    float eX = std::abs(velocity_.x) > RESTITUTION_THRESHOLD ? restitution_ : 0.0f;
    float eY = std::abs(velocity_.y) > RESTITUTION_THRESHOLD ? restitution_ : 0.0f;

    velocity_.x *= 1.0f - hitX * (1.0f + eX);
    velocity_.y *= 1.0f - hitY * (1.0f + eY);

}


Body Body::makeCircle(const int nextBodyId, const float radius, const float x, const float y) {
       return  Body{
        nextBodyId,
        radius,
        Vec2{x, y}
    };
}

void Body::handleCircleCollision(Body& b, const float& radiusSum) {

    Vec2 direction = b.position_ - position_;
    float length = std::sqrt((direction.x * direction.x) + (direction.y * direction.y));

    if (length == 0.0f) return; //avoid dividing by 0

    Vec2 normal = direction / length;

    Vec2 relVelocity = b.velocity_ - velocity_;
    float velAlongNormal = dotProduct(relVelocity, normal);

    if (velAlongNormal > 0) return; // already moving apart

    float invMassA = inverseMass();
    float invMassB = b.inverseMass(); 

    float e = std::min(restitution_, b.restitution_);

    float impulseMagnitute = -(1 + e) * velAlongNormal / (invMassA + invMassB);

    velocity_ -= impulseMagnitute * invMassA * normal;
    b.velocity_ += impulseMagnitute * invMassB * normal;

    float penetration = radiusSum - length;

    Vec2 correction = normal * penetration;

    position_ -= correction * (invMassA / (invMassA + invMassB));
    b.position_ += correction * (invMassB / (invMassA + invMassB));

}

void Body::handleCollision(Body& b) {
    const auto* cA = std::get_if<Circle>(&shape_);
    const auto* cB = std::get_if<Circle>(&b.shape_);

    if (cA && cB) {
        Vec2 d = position_ - b.position_;
        float dSq = d.x * d.x + d.y * d.y;
        float radiusSum = cA->radius + cB->radius;
        if (dSq < radiusSum * radiusSum) {
            handleCircleCollision(b, radiusSum);
        }
    }

}

void Body::drag(const Vec2& distance, const float& dt) {
    position_ += distance;
    velocity_ = distance / dt;
}
 void Body::draw() const {
    const auto& pos = position_;

    if (const auto* c = std::get_if<Circle>(&shape_)) {
        DrawCircleV({pos.x, pos.y}, c->radius, RAYWHITE);
    } else if (const auto* box = std::get_if<Box>(&shape_)) {
        DrawRectangleV({pos.x - box->width / 2, pos.y - box->height / 2}, {box->width, box->height}, RAYWHITE);
    }

}

bool Body::overlaps(const Body& b) const {
    const auto* cA = std::get_if<Circle>(&shape_);
    const auto* cB = std::get_if<Circle>(&b.shape_);

    if (cA && cB) {
        Vec2 d = position_ - b.position_;
        float dSq = d.x * d.x + d.y * d.y;
        float radiusSum = cA->radius + cB->radius;
        return dSq < radiusSum * radiusSum;
    }

    return false;
}

Body Body::makeCircle(const int nextBodyId, const float radius, const float x, const float y, const float density) {
    return Body{
        nextBodyId,
        radius,
        Vec2{x, y},
        density
    };

}
