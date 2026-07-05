#include "body.hpp"
#include "vec2.hpp"
#include <cassert>

Vec2 halfExtents(const Body& body) {
    if (const auto* c = std::get_if<Circle>(&body.shape)) {
        return {c->radius, c->radius};
    } else if (const auto* box = std::get_if<Box>(&body.shape)) {
        return {box->width / 2, box->height / 2};
    }
    assert(false && "unhandled Shape type");

    return {0, 0};

}
