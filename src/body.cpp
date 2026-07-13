#include "body.hpp"
#include <raylib.h>
#include "shape.hpp"
#include "vec2.hpp"
#include "constants.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "contact.hpp"

void Body::freeze() {
    velocity_.x = 0.0f;
    velocity_.y = 0.0f;
    angularVelocity_ = 0.0f;
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

float Body::calcInertia() const {

    if (const auto* c = std::get_if<Circle>(&shape_)) {
        return 0.5f * mass() * c->radius * c->radius;
    } else if (const auto* box = std::get_if<Box>(&shape_)) {
        return mass() * ((box->height * box->height) + (box->width * box->width)) / 12;
    }

    assert(false && "unhandled Shape type");

    return 0.0f;

}

float Body::calcInverseInertia() const {
    float i = inertia();
    return i > 0 ? 1 / i : 0.0f;
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
    angle_ = angle_ + angularVelocity_ * dt;
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

void Body::handleCircleCollision(Body& b, const Contact& contact) {
    adjustVelocity(b, contact);
    correct(b, contact.correction);
}

void Body::correct(Body& other, const Vec2& correction) {
    float inverseMassSum = inverseMass_ + other.inverseMass_;
    position_ -= correction * (inverseMass_ / inverseMassSum);
    other.position_ += correction * (other.inverseMass_ / inverseMassSum);
}

void Body::adjustVelocity(Body& other, const Contact& contact) {

    auto normal = contact.normal;

    Vec2 relVelocity = other.velocity_ - velocity_;

    float velAlongNormal = dotProduct(relVelocity, normal);

    if (velAlongNormal > 0) return; // already moving apart

    float invMassA = inverseMass();
    float invMassB = other.inverseMass(); 

    float e = collisionRestitution(other);

    float impulseMagnitute = -(1 + e) * velAlongNormal / (invMassA + invMassB);

    Vec2 J = impulseMagnitute * normal;

    velocity_ -= J * invMassA;
    angularVelocity_ -= crossProduct(contact.rA, J) * inverseInertia_;

    other.velocity_ += J * invMassB;
    other.angularVelocity_ += crossProduct(contact.rB, J) * other.inverseInertia_;

    applyFrictionImpulse(other, contact, impulseMagnitute);

}

void Body::applyFrictionImpulse(Body& other, const Contact& contact, const float& impulseMagnitute) {
    // relative velocity of the 2 surfaces at the contact point
    Vec2 relativeVelocity = (other.velocity_ + other.angularVelocity_ * perpendicular(contact.rB))
        - (velocity_ + angularVelocity_ * perpendicular(contact.rA));

    Vec2 tangent = relativeVelocity - (contact.normal * (dotProduct(relativeVelocity, contact.normal)));

    float tLen = std::sqrt(dotProduct(tangent, tangent));

    // check if there was any meaningful slide, can't use 0 due to floating point 
    if (tLen <= EPSILON) return;

    tangent /= tLen;

    //impulse needed to stop the sliding
    float jt = -dotProduct(relativeVelocity, tangent);

    jt /= inverseMass_ + other.inverseMass_
          + crossProduct(contact.rA, tangent) * crossProduct(contact.rA, tangent) * inverseInertia_
          + crossProduct(contact.rB, tangent) * crossProduct(contact.rB, tangent) * other.inverseInertia_;


    float mu = collisionFriction(other);

    jt = std::clamp(jt, -mu * impulseMagnitute, mu * impulseMagnitute);

    //apply equal and opposite tangential impulses
    Vec2 Jt = jt * tangent;
    velocity_          -= Jt * inverseMass_;
    angularVelocity_   -= crossProduct(contact.rA, Jt) * inverseInertia_;
    other.velocity_        += Jt * other.inverseMass_;
    other.angularVelocity_ += crossProduct(contact.rB, Jt) * other.inverseInertia_;

}

void Body::handleCollision(Body& b) {
    const auto* cA = std::get_if<Circle>(&shape_);
    const auto* cB = std::get_if<Circle>(&b.shape_);

    if (cA && cB) {
        if (auto contact = circleCircle(*this, b, cA->radius, cB->radius)) {
            handleCircleCollision(b, *contact);
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
        // spin indicator: line from center to rim at angle_
        Vec2 rim = pos + Vec2{std::cos(angle_), std::sin(angle_)} * c->radius;
        DrawLineEx({pos.x, pos.y}, {rim.x, rim.y}, 2.0f, RED);
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
