#include "world.hpp"
#include <raylib.h>
#include <algorithm>
#include <optional>
#include "constants.hpp"
#include "vec2.hpp"

void World::step(const float& dt) {

    integrate(dt);
    detect();
}

void World::integrate(const float& dt, MotionState &state) {
    Vec2& position = state.position;
    Vec2& velocity = state.velocity;
    velocity = velocity + gravity * dt;
    position += velocity * dt;
}

void World::integrate(const float& dt) {
    for (auto& body : bodies) {
        integrate(dt, body.state);
    }
}

void World::detect() {
    for (auto& a : bodies) {

        if (auto c = boundsContact(a)) {
            a.state.position += c->correction;
            float hitX = c->correction.x != 0;
            float hitY = c->correction.y != 0;

            float eX = a.state.velocity.x > RESTITUTION_THRESHOLD ? a.restitution : 0.0f;
            float eY = a.state.velocity.y > RESTITUTION_THRESHOLD ? a.restitution : 0.0f;

            a.state.velocity.x *= 1.0f - hitX * (1.0f + eX);
            a.state.velocity.y *= 1.0f - hitY * (1.0f + eY);


        }

        for (auto& b : bodies) {
            if (&a == &b) continue;
            if (bodiesOverlap(a, b)) {

            }
        }
    }
}

std::optional<BoundsContact> World::boundsContact(const Body& body) {
    Vec2 pos = body.state.position;
    Vec2 extents = halfExtents(body);

    Vec2 clamped = {
        std::clamp(pos.x, extents.x, width - extents.x),
        std::clamp(pos.y, extents.y, height - extents.y)
    };

    Vec2 correction = clamped - pos;

    if (correction.x == 0.0f && correction.y == 0.0f) {
        return std::nullopt;
    }

    return BoundsContact{correction};

}

bool World::bodiesOverlap(const Body& a, const Body& b) {
    const auto* cA = std::get_if<Circle>(&a.shape);
    const auto* cB = std::get_if<Circle>(&b.shape);

    if (cA && cB) {
        Vec2 d = a.state.position - b.state.position;
        float dSq = d.x * d.x + d.y * d.y;
        float radiusSum = cA->radius + cB->radius;
        return dSq < radiusSum * radiusSum;
    }

    return false;
}



void World::draw() {
    for (const auto& body : bodies) {

        const auto& pos = body.state.position;

        if (const auto* c = std::get_if<Circle>(&body.shape)) {
            DrawCircleV({pos.x, pos.y}, c->radius, RAYWHITE);
        } else if (const auto* box = std::get_if<Box>(&body.shape)) {
            DrawRectangleV({pos.x - box->width / 2, pos.y - box->height / 2}, {box->width, box->height}, RAYWHITE);
        }


    }
}
