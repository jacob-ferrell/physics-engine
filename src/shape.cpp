#include "shape.hpp"
#include <raylib.h>
#include <algorithm>
#include <cassert>
#include "constants.hpp"

Vec2 halfExtents(Body& body) {
    if (const auto* c = std::get_if<Circle>(&body.shape)) {
        return {c->radius, c->radius};
    } else if (const auto* box = std::get_if<Box>(&body.shape)) {
        return {box->width / 2, box->height / 2};
    }
    assert(false && "unhandled Shape type");

    return {0, 0};

}

float calcPositionWithinBounds(
    const float& dt,
    const float& position, 
    const float& velocity, 
    const float& edge,
    const int& bound
) {
    return std::clamp(position + velocity * dt, 0.0f + edge, static_cast<float>(bound) - edge);
}

void updatePosition(const float& dt, Body &body) {
    Vec2& position = body.state.position;
    Vec2& velocity = body.state.velocity;
    velocity = velocity + GRAVITY * dt;
    Vec2 edges = halfExtents(body);
    position.x = calcPositionWithinBounds(dt, position.x, velocity.x, edges.x, WINDOW_WIDTH);
    position.y = calcPositionWithinBounds(dt, position.y, velocity.y, edges.y, WINDOW_HEIGHT);

}

