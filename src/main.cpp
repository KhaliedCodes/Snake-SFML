#include <SFML/Graphics.hpp>
#include <iostream>

// #include "../headers/Game.hpp"
#include "../headers/Game.hpp"
using namespace sf;

int main() {
    srand(time(0));
    // Program entry point.
    Game game;  // Creating our game object.
    while (!game.GetWindow()->IsDone()) {
        // Game loop.

        game.HandleInput();
        game.Update();
        game.Render();
        game.RestartClock();
    }
    return 0;
}
