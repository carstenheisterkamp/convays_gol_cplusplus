#include <SFML/Graphics.hpp>
#include "GameOfLife.h"
#include <iostream>

int main() {
    constexpr  int WINDOW_WIDTH = 1200;
    constexpr  int WINDOW_HEIGHT = 1200;
    constexpr  int GRID_ROWS = 300;
    constexpr int GRID_COLS = 300;

    bool isLeftMousePressed = false;
    bool isRightMousePressed = false;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Comvays Game Of Life");
    window.setFramerateLimit(60);

    GameOfLife game(GRID_ROWS, GRID_COLS, window);

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    constexpr sf::Time timePerFrame = sf::seconds(1.f / 10.f);

    while (window.isOpen()) {
        if (const auto event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }

            if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mousePress->button == sf::Mouse::Button::Left) {
                    isLeftMousePressed = true;
                }
                if (mousePress->button == sf::Mouse::Button::Right) {
                    isRightMousePressed = true;
                }
                if (mousePress->button == sf::Mouse::Button::Middle) {
                    sf::Vector2i mousePixelPos = mousePress->position;

                    const int cellCol = mousePixelPos.x / (WINDOW_WIDTH / GRID_COLS);
                    const int cellRow = mousePixelPos.y / (WINDOW_HEIGHT / GRID_ROWS);
                    game.invertCell(cellRow, cellCol);
                }
            }

            if (const auto* mousePressRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mousePressRelease->button == sf::Mouse::Button::Left) {
                    isLeftMousePressed = false;
                }
                if (mousePressRelease->button == sf::Mouse::Button::Right) {
                    isRightMousePressed = false;
                }
            }
        }

        if (isLeftMousePressed) {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            int cellCol = mousePixelPos.x / (WINDOW_WIDTH / GRID_COLS);
            int cellRow = mousePixelPos.y / (WINDOW_HEIGHT / GRID_ROWS);
            game.setCellState(cellRow, cellCol, LIVE);
        }

        if (isRightMousePressed) {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            int cellCol = mousePixelPos.x / (WINDOW_WIDTH / GRID_COLS);
            int cellRow = mousePixelPos.y / (WINDOW_HEIGHT / GRID_ROWS);
            game.setCellState(cellRow, cellCol, DEATH);
        }

        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            game.update();
        }

        window.clear();
        game.draw();
        window.display();
    }

    return 0;
}