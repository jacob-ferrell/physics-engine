#include "world.hpp"
#include <raylib.h>
#include <algorithm>
#include <optional>
#include "body.hpp"
#include "vec2.hpp"

void World::step(const float& dt) {
    integrate(dt);
    detect();
}

void World::integrate(const float& dt, MotionState &state) {
    Vec2& position = state.position;
    Vec2& velocity = state.velocity;
    velocity = velocity + gravity * dt; // apply gravity
    position += velocity * dt;
}

void World::integrate(const float& dt) {
    bool mousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    Vector2 mp = GetMousePosition();
    Vec2 mousePosition = Vec2{mp.x, mp.y};

    for (auto& body : bodies) {

        if (!mousePressed) {
            selectedBody = std::nullopt;
        } else if (!selectedBody && positionWithinBody(body, mousePosition)) {
            grabBody(body, mp);
            continue;
        }     

        bool bodyIsSelected = selectedBody && selectedBody->bodyId == body.id;

        if (bodyIsSelected) {
            dragBody(body, mousePosition, dt);
        } else {
            integrate(dt, body.state);
        }


    }
}

void World::detect() {


    for (auto& a : bodies) {

    
        if (auto c = boundsContact(a)) {
            bounceBody(*c, a);
        }

        for (auto& b : bodies) {
            if (&a == &b) continue;
            handleCollision(a, b);
        }

    }
}

std::optional<Vec2> World::boundsContact(const Body& body) {
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

    return correction;

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

void World::grabBody(Body& body, const Vector2& mousePosition) {
    freeze(body);
    selectedBody = MouseSelection{
        body.id, 
        Vec2{mousePosition.x, mousePosition.y}
    };
}

void World::dragBody(Body& body, const Vec2& mousePosition, const float& dt) {
    Vec2 d = mousePosition - selectedBody->lastMousePosition;
    body.state.position += d;
    body.state.velocity = d / dt;
    selectedBody->lastMousePosition = mousePosition;
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

void World::addBody(const Body& body) {
    bodies.push_back(body);
}
