#include "../headers/Game.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

Game::~Game() {}
Game::Game()
    : m_window("Snake", sf::Vector2u(800, 600)),
      m_snake(m_world.GetBlockSize()),
      m_world(sf::Vector2u(800, 600)) {}
void Game::Update() {
    float timestep = 1.0f / m_snake.GetSpeed();
    if (m_elapsed.asSeconds() >= timestep &&
        m_snake.GetDirection() != Direction::None) {
        m_snake.Tick();
        m_world.Update(m_snake);
        if (m_snake.HasLost()) {
            m_snake.Reset();
        }
    }
    if (m_elapsed.asSeconds() >= timestep) {
        m_elapsed -= sf::seconds(timestep);
    }
    m_window.Update();  // Update window events.
}

void Game::Render() {
    m_window.BeginDraw();  // Clear.
    m_world.Render(m_window.m_window);
    m_snake.Render(m_window.m_window);  // Display.
    m_snake.DisplayScore(m_window.m_window);
    m_window.EndDraw();
}

void Game::HandleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) &&
        m_snake.GetDirection() != Direction::Down) {
        m_snake.SetDirection(Direction::Up);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
               m_snake.GetDirection() != Direction::Up) {
        m_snake.SetDirection(Direction::Down);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
               m_snake.GetDirection() != Direction::Right) {
        m_snake.SetDirection(Direction::Left);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
               m_snake.GetDirection() != Direction::Left) {
        m_snake.SetDirection(Direction::Right);
    }
}

sf::Time Game::GetElapsed() { return m_elapsed; }
void Game::RestartClock() { m_elapsed += m_clock.restart(); }

GameWindow* Game::GetWindow() { return &m_window; }
