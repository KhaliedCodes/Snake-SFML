
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

#include "Snake.hpp"
#include "Window.hpp"
#include "World.hpp"

void Game();
void HandleInput();
void UpdateGame();
void RenderGame();
sf::Time GetElapsed();
void RestartClock();
void MoveMushroom();
sf::Time m_elapsed;
void Game() {
    GameWindow("Snake", sf::Vector2u(800, 600));
    World(sf::Vector2u(800, 600));
    Snake(GetBlockSize());
}
void UpdateGame() {
    float timestep = 1.0f / GetSpeed();
    if (m_elapsed.asSeconds() >= timestep &&
        GetDirection() != Direction::None) {
        Tick();
        UpdateWorld();
        if (HasLost()) {
            Reset();
        }
    }
    if (m_elapsed.asSeconds() >= timestep) {
        m_elapsed -= sf::seconds(timestep);
    }
    UpdateWindow();  // Update window events.
}

void RenderGame() {
    BeginDraw();  // Clear.
    RenderWorld(m_window);
    RenderSnake(m_window);  // Display.
    DisplayScore(m_window);
    EndDraw();
}

void HandleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        GetDirection() != Direction::Down) {
        SetDirection(Direction::Up);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
               GetDirection() != Direction::Up) {
        SetDirection(Direction::Down);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
               GetDirection() != Direction::Right) {
        SetDirection(Direction::Left);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
               GetDirection() != Direction::Left) {
        SetDirection(Direction::Right);
    }
}

sf::Time GetElapsed() { return m_elapsed; }
void RestartClock() { m_elapsed += m_clock.restart(); }
