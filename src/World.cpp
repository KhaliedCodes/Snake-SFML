#include "../headers/World.hpp"

#include <string.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
World::World(sf::Vector2u l_windSize) {
    m_blockSize = 16;
    m_windowSize = l_windSize;
    RespawnApple();
    RespawnShield();
    GenerateMovingWalls();
    m_horizontalWallDirection = Direction::Right;
    m_verticalWallDirection = Direction::Down;
    m_slowShape.setPosition(-100, -100);
    m_appleShape.setFillColor(sf::Color::Red);
    m_appleShape.setRadius(m_blockSize / 2);
    m_shieldShape.setFillColor(sf::Color::Blue);
    m_shieldShape.setSize(sf::Vector2f(m_blockSize, m_blockSize));
    m_slowShape.setFillColor(sf::Color::Green);
    m_slowShape.setRadius(m_blockSize / 2);
    for (int i = 0; i < 4; ++i) {
        m_bounds[i].setFillColor(sf::Color(150, 0, 0));
        if (!((i + 1) % 2)) {
            m_bounds[i].setSize(sf::Vector2f(m_windowSize.x, m_blockSize));
        } else {
            m_bounds[i].setSize(sf::Vector2f(m_blockSize, m_windowSize.y));
        }
        if (i < 2) {
            m_bounds[i].setPosition(0, 0);
        } else {
            m_bounds[i].setOrigin(m_bounds[i].getSize());
            m_bounds[i].setPosition(sf::Vector2f(m_windowSize));
        }
    }

    ReadWorld();
    m_clock.restart();
}

void World::RespawnShield() {
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;
    m_shield = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
    bool overlapped = false;
    do {
        bool overlapped = CheckCollisionWithWalls(m_shield);
    } while (overlapped);
    m_shieldShape.setPosition(m_shield.x * m_blockSize,
                              m_shield.y * m_blockSize);
}

void World::RespawnApple() {
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;
    m_apple = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
    bool overlapped = false;
    do {
        cout << "inside loop" << endl;
        bool overlapped = CheckCollisionWithWalls(m_apple);
    } while (overlapped);
    m_appleShape.setPosition(m_apple.x * m_blockSize, m_apple.y * m_blockSize);
}

void World::RespawnSlow() {
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;
    m_slow = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
    bool overlapped = false;
    do {
        bool overlapped = CheckCollisionWithWalls(m_slow);
    } while (overlapped);
    m_slowShape.setPosition(m_slow.x * m_blockSize, m_slow.y * m_blockSize);
}

bool World::CheckCollisionWithWalls(sf::Vector2i& m) {
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (m.x == (grid[i][j]->getPosition().x / m_blockSize) &&
                m.y == (grid[i][j]->getPosition().y / m_blockSize)) {
                m = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
                cout << "iI'M INNN" << endl;
                return true;
            }
        }
    }
    return false;
}

void World::Update(Snake& l_player) {
    MoveWalls();
    if (l_player.GetPosition() == m_apple) {
        cout << "eated?" << endl;
        l_player.Extend();
        l_player.IncreaseScore();
        l_player.IncreaseSpeed();
        RespawnApple();
    }

    if (l_player.GetPosition() == m_shield) {
        l_player.SetShield();
        m_shieldShape.setPosition(-111, -111);
        m_shield = sf::Vector2(-111, -111);
    }
    if (l_player.GetPosition() == m_slow) {
        l_player.DecreaseSpeed();
        m_slowShape.setPosition(-111, -111);
        m_slow = sf::Vector2(-111, -111);
    }
    auto elapsed = m_clock.getElapsedTime().asSeconds();
    if (((int)elapsed % 30) == 0 && (int)elapsed != 0 &&
        m_shieldShape.getPosition().x < 0) {
        RespawnShield();
    }
    if (((int)elapsed % 30) == 0 && (int)elapsed != 0 &&
        m_slowShape.getPosition().x < 0) {
        RespawnSlow();
    }
    int gridSize_x = m_windowSize.x / m_blockSize;
    int gridSize_y = m_windowSize.y / m_blockSize;
    if (l_player.GetPosition().x <= 0 || l_player.GetPosition().y <= 0 ||
        l_player.GetPosition().x >= gridSize_x - 1 ||
        l_player.GetPosition().y >= gridSize_y - 1) {
        if (l_player.HasShield()) {
            l_player.LoseShield();
            l_player.SetDirection(Direction::None);
            l_player.MoveReverse();
        } else {
            l_player.Lose();
            RespawnApple();
            RespawnShield();
            GenerateMovingWalls();
            m_clock.restart();
        }
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            if (l_player.GetPosition().x ==
                    (grid[i][j]->getPosition().x / m_blockSize) &&
                l_player.GetPosition().y ==
                    (grid[i][j]->getPosition().y / m_blockSize))
                if (l_player.HasShield()) {
                    l_player.LoseShield();
                    l_player.SetDirection(Direction::None);
                    l_player.MoveReverse();
                } else {
                    GenerateMovingWalls();
                    RespawnApple();
                    RespawnShield();
                    m_clock.restart();
                    l_player.Lose();
                }
        }
    }
    for (int i = 0; i < m_verticalWallShape.size(); i++) {
        for (int j = 0; j < l_player.GetSnakeBody().size(); j++) {
            if (l_player.GetSnakeBody()[j].position.x ==
                    (m_verticalWallShape[i]->getPosition().x / m_blockSize) &&
                l_player.GetSnakeBody()[j].position.y ==
                    (m_verticalWallShape[i]->getPosition().y / m_blockSize))
                if (l_player.HasShield()) {
                    l_player.LoseShield();
                    l_player.SetDirection(Direction::None);
                    l_player.MoveReverse();
                } else {
                    RespawnApple();
                    RespawnShield();
                    m_clock.restart();
                    l_player.Lose();
                }
        }
    }
    for (int i = 0; i < m_horizontalWallShape.size(); i++) {
        for (int j = 0; j < l_player.GetSnakeBody().size(); j++) {
            if (l_player.GetSnakeBody()[j].position.x ==
                    (m_horizontalWallShape[i]->getPosition().x / m_blockSize) &&
                l_player.GetSnakeBody()[j].position.y ==
                    (m_horizontalWallShape[i]->getPosition().y / m_blockSize))
                if (l_player.HasShield()) {
                    l_player.LoseShield();
                    l_player.SetDirection(Direction::None);
                    l_player.MoveReverse();
                } else {
                    RespawnApple();
                    RespawnShield();
                    m_clock.restart();
                    l_player.Lose();
                }
        }
    }
}

void World::Render(sf::RenderWindow& l_window) {
    for (int i = 0; i < 4; ++i) {
        l_window.draw(m_bounds[i]);
    }
    for (int i = 0; i < m_verticalWallShape.size(); ++i) {
        l_window.draw((*m_verticalWallShape[i]));
    }
    for (int i = 0; i < m_horizontalWallShape.size(); ++i) {
        l_window.draw((*m_horizontalWallShape[i]));
    }
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[i].size(); j++) {
            l_window.draw((*grid[i][j]));
        }
    }
    l_window.draw(m_appleShape, sf::BlendAdd);
    l_window.draw(m_shieldShape, sf::BlendAdd);
    l_window.draw(m_slowShape, sf::BlendAdd);
}

int World::GetBlockSize() { return m_blockSize; }
World::~World() {}

void World::ReadWorld() {
    const string& path = "../static/grid.txt";
    ifstream myfile(path, ios_base::in);

    if (!myfile.is_open()) {
        cerr << "Error: Could not open file." << endl;
        return;
    }

    string line;
    int row = 0;
    int winY = 0;
    while (getline(myfile, line) && winY < m_windowSize.y) {
        grid.push_back(std::vector<RectangleShape*>());
        for (int col = 0, winX = 0; winX < m_windowSize.x && line[col] != '\0';
             col++, winX += m_blockSize) {
            if (line[col] == '#') {
                auto block = new RectangleShape();
                grid[row].push_back(block);
                (*block).setPosition(sf::Vector2f(winX, winY));
                (*block).setSize(sf::Vector2f(m_blockSize, m_blockSize));
                (*block).setFillColor(sf::Color(150, 0, 0));
            }
        }
        row++;
        winY += m_blockSize;
    }

    myfile.close();
}

void World::GenerateMovingWalls() {
    m_verticalWallShape.clear();
    m_horizontalWallShape.clear();
    int maxX = (m_windowSize.x / m_blockSize) - 2;
    int maxY = (m_windowSize.y / m_blockSize) - 2;
    m_verticalWall = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
    auto block = new RectangleShape();
    (*block).setSize(sf::Vector2f(m_blockSize, m_blockSize));
    (*block).setFillColor(sf::Color(150, 0, 0));
    m_verticalWallShape.push_back(block);
    (*m_verticalWallShape.front())
        .setPosition(m_verticalWall.x * m_blockSize,
                     m_verticalWall.y * m_blockSize);
    for (int i = 1; i < 5; i++) {
        auto block = new RectangleShape();
        m_verticalWallShape.push_back(block);
        (*block).setPosition(sf::Vector2f(
            (m_verticalWallShape[i - 1]->getPosition().x),
            (m_verticalWallShape[i - 1]->getPosition().y + m_blockSize)));
        (*block).setSize(sf::Vector2f(m_blockSize, m_blockSize));
        (*block).setFillColor(sf::Color(150, 0, 0));
    }

    m_horizontalWall = sf::Vector2i(rand() % maxX + 1, rand() % maxY + 1);
    block = new RectangleShape();
    (*block).setSize(sf::Vector2f(m_blockSize, m_blockSize));
    (*block).setFillColor(sf::Color(150, 0, 0));
    m_horizontalWallShape.push_back(block);
    (*m_horizontalWallShape.front())
        .setPosition(m_horizontalWall.x * m_blockSize,
                     m_horizontalWall.y * m_blockSize);
    for (int i = 1; i < 5; i++) {
        auto block = new RectangleShape();
        m_horizontalWallShape.push_back(block);
        (*block).setPosition(sf::Vector2f(
            (m_horizontalWallShape[i - 1]->getPosition().x + m_blockSize),
            (m_horizontalWallShape[i - 1]->getPosition().y)));
        (*block).setSize(sf::Vector2f(m_blockSize, m_blockSize));
        (*block).setFillColor(sf::Color(150, 0, 0));
    }
}
void World::MoveWalls() {
    if (m_verticalWallDirection == Direction::Down) {
        (*m_verticalWallShape.back())
            .setPosition(Vector2f(
                (*m_verticalWallShape.back()).getPosition().x,
                (*m_verticalWallShape.back()).getPosition().y + m_blockSize));
        for (int i = m_verticalWallShape.size() - 2; i >= 0; --i) {
            (*m_verticalWallShape[i])
                .setPosition(
                    Vector2f((*m_verticalWallShape[i + 1]).getPosition().x,
                             (*m_verticalWallShape[i + 1]).getPosition().y -
                                 m_blockSize));
        }
        if (((*m_verticalWallShape.back()).getPosition().y + m_blockSize >=
             m_windowSize.y)) {
            m_verticalWallDirection = Direction::Up;
        }
    }
    if (m_verticalWallDirection == Direction::Up) {
        (*m_verticalWallShape.front())
            .setPosition(Vector2f(
                (*m_verticalWallShape.front()).getPosition().x,
                (*m_verticalWallShape.front()).getPosition().y - m_blockSize));
        for (int i = 1; i < m_verticalWallShape.size(); ++i) {
            (*m_verticalWallShape[i])
                .setPosition(
                    Vector2f((*m_verticalWallShape[i - 1]).getPosition().x,
                             (*m_verticalWallShape[i - 1]).getPosition().y +
                                 m_blockSize));
        }
        if (((*m_verticalWallShape.front()).getPosition().y - m_blockSize <=
             0)) {
            m_verticalWallDirection = Direction::Down;
        }
    }

    if (m_horizontalWallDirection == Direction::Right) {
        (*m_horizontalWallShape.back())
            .setPosition(Vector2f(
                (*m_horizontalWallShape.back()).getPosition().x + m_blockSize,
                (*m_horizontalWallShape.back()).getPosition().y));
        for (int i = m_horizontalWallShape.size() - 2; i >= 0; --i) {
            (*m_horizontalWallShape[i])
                .setPosition(
                    Vector2f((*m_horizontalWallShape[i + 1]).getPosition().x -
                                 m_blockSize,
                             (*m_horizontalWallShape[i + 1]).getPosition().y));
        }
        if (((*m_horizontalWallShape.back()).getPosition().x + m_blockSize >=
             m_windowSize.x)) {
            m_horizontalWallDirection = Direction::Left;
        }
    }
    if (m_horizontalWallDirection == Direction::Left) {
        (*m_horizontalWallShape.front())
            .setPosition(Vector2f(
                (*m_horizontalWallShape.front()).getPosition().x - m_blockSize,
                (*m_horizontalWallShape.front()).getPosition().y));
        for (int i = 1; i < m_horizontalWallShape.size(); ++i) {
            (*m_horizontalWallShape[i])
                .setPosition(
                    Vector2f((*m_horizontalWallShape[i - 1]).getPosition().x +
                                 m_blockSize,
                             (*m_horizontalWallShape[i - 1]).getPosition().y));
        }
        if (((*m_horizontalWallShape.front()).getPosition().x - m_blockSize <=
             0)) {
            m_horizontalWallDirection = Direction::Right;
        }
    }
}
