#include <raylib.h>
#include "world.hpp"
#include "constants.hpp"

int main() {
    World world{};
    while (true) {
        InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Physics");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            float dt = GetFrameTime();
            BeginDrawing();
            ClearBackground(BLACK);
            world.draw();
            world.step(dt);
            EndDrawing();
        }
    }
    return 0;
}
