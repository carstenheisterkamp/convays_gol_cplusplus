#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

enum CellStates { DEATH, LIVE };

class GameOfLife {
private:
    sf::RenderWindow& m_window;
    const int m_rows;
    const int m_cols;
    void initializeGrid();
    void initializeVertices();

    std::vector<std::vector<CellStates>> m_grid;
    sf::VertexArray m_vertices;

    [[nodiscard]] int countLiveNeighbors(int r, int c) const;

public:
    GameOfLife(int rows, int cols, sf::RenderWindow& window);
    void invertCell(int row, int col);
    void setCellState(int row, int col, CellStates state);

    void update();
    void draw();
};