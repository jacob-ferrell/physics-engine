#include "shape.hpp"
#include <raylib.h>
#include <algorithm>
#include <cassert>
#include "body.hpp"



float calcPositionWithinBounds(
    const float& dt,
    const float& position, 
    const float& velocity, 
    const float& edge,
    const int& bound
) {
    return std::clamp(position + velocity * dt, 0.0f + edge, static_cast<float>(bound) - edge);
}

