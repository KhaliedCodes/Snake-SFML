#include <SFML/Graphics.hpp>

#include "Snake.hpp"
#include "Window.hpp"
#include "World.hpp"

class Game {
  public:
    Game();
    ~Game();
    void HandleInput();
    void Update();
    void Render();
    GameWindow* GetWindow();
    sf::Time GetElapsed();
    void RestartClock();

  private:
    World m_world;
    Snake m_snake;
    void MoveMushroom();
    GameWindow m_window;
    sf::Clock m_clock;
    sf::Time m_elapsed;
};