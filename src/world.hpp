#pragma once

#include <vector>
#include "shape.hpp"
class World {
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
    void step(float& dt);
    void draw();

};
