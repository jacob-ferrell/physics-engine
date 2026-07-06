#include "shape.hpp"
#include <raylib.h>
#include <cassert>
#include <cmath>
#include <variant>


float area(const Shape& shape) {
    if (auto* c = std::get_if<Circle>(&shape)) {
        return (float)M_PI * c->radius * c->radius;
    }

    if (auto* b = std::get_if<Box>(&shape)) {
        return b->width * b->height;
    }

    assert(false && "unhandled shape");

    return 0.0f;
    
}
