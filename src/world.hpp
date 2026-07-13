#pragma once

#include <raylib.h>
#include <optional>
#include <vector>
#include "constants.hpp"
#include "body.hpp"
#include "vec2.hpp"

struct MouseSelection {
    int bodyId;
    Vec2 lastMousePosition;
};

class World {

    const float height = WINDOW_HEIGHT;
    const float width = WINDOW_WIDTH;
    const Vec2 gravity = GRAVITY;

    int nextBodyId = 0;

    std::vector<Body> bodies{};

    std::optional<MouseSelection> selectedBody;


public:
    World() : World(GRAVITY) {};
    explicit World(Vec2 g) : gravity(g) {
        addBody(Body::makeCircle(nextBodyId++, 20, 100, 300));
        addBody(Body::makeCircle(nextBodyId++, 50, 400, 500));
    };
    void step(const float& dt);
    void draw();

private:
    void integrate(const float& dt);
    void detect();
    std::optional<Vec2> boundsContact(const Body& body);
    bool bodiesOverlap(const Body& a, const Body& b);
    void grabBody(Body& body, const Vector2& mousePosition);
    void dragBody(Body& body, const Vec2& mousePosition, const float& dt);
    void addBody(const Body& body);
};

