#include "world.hpp"
#include <raylib.h>
#include "shape.hpp"

void World::step(float& dt) {
    for (auto& body : bodies) {
        updatePosition(dt, body);
    }
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
