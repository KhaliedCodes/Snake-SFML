#include <SFML/Graphics.hpp>
#include <iostream>

// #include "../headers/Game.hpp"
#include "Game.hpp"
using namespace sf;

int main() {
    srand(time(0));
    // Program entry point.
    Game();  // Creating our game object.
    while (!IsDone()) {
        // Game loop.

        HandleInput();
        UpdateGame();
        RenderGame();
        RestartClock();
    }
    return 0;
}
