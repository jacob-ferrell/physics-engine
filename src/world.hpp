#pragma once

#include <raylib.h>
#include <optional>
#include <vector>
#include "constants.hpp"
#include "shape.hpp"
#include "body.hpp"
class World {

    const float height = WINDOW_HEIGHT;
    const float width = WINDOW_WIDTH;
    const Vec2 gravity = GRAVITY;

    Circle circle{
        .radius = 20
    };

    Body circleBody{
        .state = {
            .position = Vec2{400,  300}
        },
        .shape = circle
    };

    std::vector<Body> bodies{};

public:
    World() : World(GRAVITY) {};
    explicit World(Vec2 g) : gravity(g) {
        bodies.push_back(makeCircle(20, 100, 300));
        bodies.push_back(makeCircle(50, 400, 500));
    };
    void step(const float& dt);
    void draw();

private:
    void integrate(const float& dt, MotionState &state);
    void integrate(const float& dt);
    void detect();
    std::optional<Vec2> boundsContact(const Body& body);
    bool bodiesOverlap(const Body& a, const Body& b);
};

