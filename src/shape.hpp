#pragma once 

#include <variant>

struct Circle {
    float radius;
};

struct Box {
    float width;
    float height;
};

using Shape = std::variant<Circle, Box>;

float area(const Shape& shape);
