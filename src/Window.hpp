#include <SFML/Graphics.hpp>
#include <iostream>
void GameWindow();
void GameWindow(const std::string& l_title, const sf::Vector2u& l_size);
void BeginDraw();  // Clear the window.
void EndDraw();    // Display the changes.
void UpdateWindow();
bool IsDone();
sf::RenderWindow m_window;
bool IsFullscreen();
sf::Vector2u GetWindowSize();
void ToggleFullscreen();
void Draw(sf::Drawable& l_drawable);
void Setup(const std::string& l_title, const sf::Vector2u& l_size);
void Destroy();
void Create();
sf::Vector2u m_windowSize;
std::string m_windowTitle;
bool m_isDone;
bool m_isFullscreen;
void GameWindow() { Setup("Window", sf::Vector2u(640, 480)); }
void GameWindow(const std::string& l_title, const sf::Vector2u& l_size) {
    Setup(l_title, l_size);
}
void Setup(const std::string& l_title, const sf::Vector2u& l_size) {
    m_windowTitle = l_title;
    m_windowSize = l_size;
    m_isFullscreen = false;
    m_isDone = false;
    Create();
}

void Create() {
    auto style = (m_isFullscreen ? sf::Style::Fullscreen : sf::Style::Close);
    m_window.create({m_windowSize.x, m_windowSize.y, 32}, m_windowTitle, style);
}
void Destroy() { m_window.close(); }

void UpdateWindow() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_isDone = true;
        } else if (event.type == sf::Event::KeyPressed &&
                   event.key.code == sf::Keyboard::F5) {
            ToggleFullscreen();
        }
    }
}

void ToggleFullscreen() {
    m_isFullscreen = !m_isFullscreen;
    Destroy();
    Create();
}

bool IsDone() { return m_isDone; }
bool IsFullscreen() { return m_isFullscreen; }
sf::Vector2u GetWindowSize() { return m_windowSize; }
void Draw(sf::Drawable& l_drawable) { m_window.draw(l_drawable); }

void BeginDraw() { m_window.clear(sf::Color::Black); }
void EndDraw() { m_window.display(); }