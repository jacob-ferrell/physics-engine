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

void World::integrate(const float& dt) {
    bool mousePressed = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    Vector2 mp = GetMousePosition();
    Vec2 mousePosition = Vec2{mp.x, mp.y};

    if (!mousePressed) {
        selectedBody = std::nullopt;
    }

    for (auto& body : bodies) {

        if (mousePressed && !selectedBody && body.containsPosition(mousePosition)) {
            grabBody(body, mp);
            continue;
        }     

        bool bodyIsSelected = selectedBody && selectedBody->bodyId == body.id();

        if (bodyIsSelected) {
            dragBody(body, mousePosition, dt);
        } else {
            body.integrate(dt, gravity);
        }


    }
}

void World::detect() {


    for (auto& a : bodies) {
    
        if (auto c = boundsContact(a)) {
            a.bounce(*c);
        }

        for (auto& b : bodies) {
            if (&a == &b) continue;
            a.handleCollision(b);
        }

    }
}

std::optional<Vec2> World::boundsContact(const Body& body) {
    Vec2 pos = body.position();
    Vec2 extents = body.halfExtents();

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

void World::grabBody(Body& body, const Vector2& mousePosition) {
    body.freeze();
    selectedBody = MouseSelection{
        body.id(), 
        Vec2{mousePosition.x, mousePosition.y}
    };
}

void World::dragBody(Body& body, const Vec2& mousePosition, const float& dt) {
    Vec2 d = mousePosition - selectedBody->lastMousePosition;
    body.drag(d, dt);
    selectedBody->lastMousePosition = mousePosition;
}

void World::draw() {
    for (const auto& body : bodies) {
        body.draw();
    }
}

void World::addBody(const Body& body) {
    bodies.push_back(body);
}
