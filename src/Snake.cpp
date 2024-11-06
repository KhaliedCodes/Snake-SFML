#include "../headers/Snake.hpp"
Snake::Snake(int l_blockSize) {
    m_size = l_blockSize;
    m_bodyRect.setSize(sf::Vector2f(m_size - 1, m_size - 1));
    Reset();
}
Snake::~Snake() {}

void Snake::SetDirection(Direction l_dir) { m_dir = l_dir; }
void Snake::Reset() {
    m_snakeBody.clear();
    m_snakeBody.push_back(SnakeSegment(5, 7));
    m_snakeBody.push_back(SnakeSegment(5, 6));
    m_snakeBody.push_back(SnakeSegment(5, 5));
    SetDirection(Direction::None);  // Start off still.
    m_speed = 10;
    m_lives = 3;
    m_score = 0;
    m_lost = false;
    shield = false;
}

Direction Snake::GetDirection() { return m_dir; }
int Snake::GetSpeed() { return m_speed; }
sf::Vector2i Snake::GetPosition() {
    return (!m_snakeBody.empty() ? m_snakeBody.front().position
                                 : sf::Vector2i(1, 1));
}
int Snake::GetLives() { return m_lives; }
int Snake::GetScore() { return m_score; }
void Snake::IncreaseScore() { m_score += 10; }
bool Snake::HasLost() { return m_lost; }
void Snake::Lose() { m_lost = true; }
void Snake::ToggleLost() { m_lost = !m_lost; }

void Snake::Extend() {
    if (m_snakeBody.empty()) {
        return;
    }
    SnakeSegment& tail_head = m_snakeBody[m_snakeBody.size() - 1];
    if (m_snakeBody.size() > 1) {
        SnakeSegment& tail_bone = m_snakeBody[m_snakeBody.size() - 2];
        if (tail_head.position.x == tail_bone.position.x) {
            if (tail_head.position.y > tail_bone.position.y) {
                m_snakeBody.push_back(SnakeSegment(tail_head.position.x,
                                                   tail_head.position.y + 1));
            } else {
                m_snakeBody.push_back(SnakeSegment(tail_head.position.x,
                                                   tail_head.position.y - 1));
            }
        } else if (tail_head.position.y == tail_bone.position.y) {
            if (tail_head.position.x > tail_bone.position.x) {
                m_snakeBody.push_back(SnakeSegment(tail_head.position.x + 1,
                                                   tail_head.position.y));
            } else {
                m_snakeBody.push_back(SnakeSegment(tail_head.position.x - 1,
                                                   tail_head.position.y));
            }
        }
    } else {
        if (m_dir == Direction::Up) {
            m_snakeBody.push_back(
                SnakeSegment(tail_head.position.x, tail_head.position.y + 1));
        } else if (m_dir == Direction::Down) {
            m_snakeBody.push_back(
                SnakeSegment(tail_head.position.x, tail_head.position.y - 1));
        } else if (m_dir == Direction::Left) {
            m_snakeBody.push_back(
                SnakeSegment(tail_head.position.x + 1, tail_head.position.y));
        } else if (m_dir == Direction::Right) {
            m_snakeBody.push_back(
                SnakeSegment(tail_head.position.x - 1, tail_head.position.y));
        }
    }
}

void Snake::Tick() {
    if (m_snakeBody.empty()) {
        return;
    }
    if (m_dir == Direction::None) {
        return;
    }
    Move();
    CheckCollision();
}

void Snake::Move() {
    for (int i = m_snakeBody.size() - 1; i > 0; --i) {
        m_snakeBody[i].position = m_snakeBody[i - 1].position;
    }
    if (m_dir == Direction::Left) {
        --m_snakeBody[0].position.x;
    } else if (m_dir == Direction::Right) {
        ++m_snakeBody[0].position.x;
    } else if (m_dir == Direction::Up) {
        --m_snakeBody[0].position.y;
    } else if (m_dir == Direction::Down) {
        ++m_snakeBody[0].position.y;
    }
}

void Snake::MoveReverse() {
    for (int i = 0; i < m_snakeBody.size() - 1; ++i) {
        m_snakeBody[i].position = m_snakeBody[i + 1].position;
    }
}

void Snake::CheckCollision() {
    if (m_snakeBody.size() < 5) {
        return;
    }
    SnakeSegment& head = m_snakeBody.front();
    for (auto itr = m_snakeBody.begin() + 1; itr != m_snakeBody.end(); ++itr) {
        if (itr->position == head.position) {
            auto segments = m_snakeBody.end() - itr;
            Cut(segments);
            break;
        }
    }
}

void Snake::Cut(int l_segments) {
    for (int i = 0; i < l_segments; ++i) {
        m_snakeBody.pop_back();
    }
    m_score /= 2;
}

void Snake::Render(sf::RenderWindow& l_window) {
    if (m_snakeBody.empty()) {
        return;
    }
    auto head = m_snakeBody.begin();
    if (HasShield()) {
        m_bodyRect.setFillColor(sf::Color::Cyan);
    } else {
        m_bodyRect.setFillColor(sf::Color::Yellow);
    }
    m_bodyRect.setPosition(head->position.x * m_size,
                           head->position.y * m_size);
    l_window.draw(m_bodyRect);
    if (HasShield()) {
        m_bodyRect.setFillColor(sf::Color::Blue);
    } else {
        m_bodyRect.setFillColor(sf::Color::Green);
    }
    for (auto itr = m_snakeBody.begin() + 1; itr != m_snakeBody.end(); ++itr) {
        m_bodyRect.setPosition(itr->position.x * m_size,
                               itr->position.y * m_size);
        l_window.draw(m_bodyRect);
    }
}

bool Snake::HasShield() { return shield; }

void Snake::SetShield() { shield = true; }

void Snake::LoseShield() { shield = false; }

void Snake::IncreaseSpeed() { m_speed++; }
void Snake::DecreaseSpeed() {
    m_speed -= 5;
    m_speed < 10 ? m_speed = 10 : m_speed;
}

void Snake::DisplayScore(sf::RenderWindow& l_window) {
    sf::Font font;
    if (!font.loadFromFile("../static/perpetua.ttf")) return;

    sf::Text scoreText = sf::Text();
    scoreText.setFont(font);  // font is a sf::Font
    scoreText.setString("Score: " + std::to_string(m_score));
    scoreText.setCharacterSize(24);  // in pixels, not points!
    scoreText.setFillColor(sf::Color::Green);
    scoreText.setOutlineThickness(1);
    scoreText.setOutlineColor(sf::Color::Yellow);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(sf::Vector2f(16, 16));
    l_window.draw(scoreText);

    sf::Text speedText = sf::Text();
    speedText.setFont(font);  // font is a sf::Font
    speedText.setString("Speed: " + std::to_string(m_speed));
    speedText.setCharacterSize(24);  // in pixels, not points!
    speedText.setFillColor(sf::Color::Green);
    speedText.setOutlineThickness(1);
    speedText.setOutlineColor(sf::Color::Yellow);
    speedText.setStyle(sf::Text::Bold);
    speedText.setPosition(sf::Vector2f(16, 48));
    l_window.draw(speedText);
}

SnakeContainer Snake::GetSnakeBody() { return m_snakeBody; }