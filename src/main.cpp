#include <raylib.h>
#include <cstring>
#include "world.hpp"
#include "constants.hpp"
#include <iostream>

// Prompt the user for gravity components, showing realistic reference values.
// Returns the chosen gravity, or the default if input is invalid.
static Vec2 promptGravity() {
    Vec2 g = GRAVITY;   // start from the default so we can fall back to it

    std::cout << "Configure gravity (acceleration in px/s^2; +y is downward)\n"
                "  y (vertical):   Earth ~1000, Mars ~370, Moon ~160, zero-g 0\n"
                "  x (horizontal): 0 for none; +/-200 for a light sideways drift\n";

    std::cout << "  x [" << g.x << "]: ";
    std::cin  >> g.x;
    std::cout << "  y [" << g.y << "]: ";
    std::cin  >> g.y;

    if (!std::cin) {                 // parse failed (non-numeric input)
        std::cout << "Invalid input; using defaults.\n";
        g = GRAVITY;
    }
    return g;
}

int main(int argc, char** argv) {

    Vec2 gravity = GRAVITY;

    for (int i = 0; i < argc; i++) {
        if (std::strcmp("-g", argv[i]) == 0) {
            gravity = promptGravity();
        }
    }

    World world{gravity};
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics");
    SetTargetFPS(144);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(BLACK);
        world.draw();
        world.step(dt);
        EndDrawing();
    }

    return 0;
}
