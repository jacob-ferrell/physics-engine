#pragma once

#include <optional>
#include <vector>
#include "constants.hpp"
#include "shape.hpp"
#include "body.hpp"

struct BoundsContact {
    Vec2 correction{0, 0};
};

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

    std::vector<Body> bodies{circleBody};

public:
    World() = default;
    explicit World(Vec2 g) : gravity(g) {};
    void step(const float& dt);
    void draw();

private:
    void integrate(const float& dt, MotionState &state);
    void integrate(const float& dt);
    void detect();
    std::optional<BoundsContact> boundsContact(const Body& body);
    bool bodiesOverlap(const Body& a, const Body& b);
};

