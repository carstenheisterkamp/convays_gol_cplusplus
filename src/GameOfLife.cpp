#include "GameOfLife.h"
#include <random> // For high-quality random numbers

// Constructor
GameOfLife::GameOfLife(int rows, int cols, sf::RenderWindow& window)
    : m_window(window), m_rows(rows), m_cols(cols)
{
    initializeGrid();
    initializeVertices();
}

void GameOfLife::initializeGrid() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    m_grid.resize(m_rows, std::vector<CellStates>(m_cols));
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            m_grid[r][c] = static_cast<CellStates>(distrib(gen));
        }
    }
}

void GameOfLife::initializeVertices() {
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(m_rows * m_cols * 6);
}

[[nodiscard]] int GameOfLife::countLiveNeighbors(const int r, const int c) const {
    int lives = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int row = (r + i + m_rows) % m_rows;
            int col = (c + j + m_cols) % m_cols;
            lives += m_grid[row][col];
        }
    }
    return lives;
}

void GameOfLife::invertCell(const int row, const int col) {
    if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
        m_grid[row][col] = (m_grid[row][col] == LIVE) ? DEATH : LIVE;
    }
}

void GameOfLife::setCellState(const int row, const int col, const CellStates state) {
    if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
        m_grid[row][col] = state;
    }
}

void GameOfLife::update() {
    std::vector<std::vector<CellStates>> nextGrid = m_grid;

    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            int liveNeighbors = countLiveNeighbors(r, c);
            if (m_grid[r][c] == LIVE && (liveNeighbors < 2 || liveNeighbors > 3)) {
                nextGrid[r][c] = DEATH;
            } else if (m_grid[r][c] == DEATH && liveNeighbors == 3) {
                nextGrid[r][c] = LIVE;
            }
        }
    }
    m_grid = nextGrid;
}


void GameOfLife::draw() {
    const sf::Vector2f windowSizeF(m_window.getSize());
    const sf::Vector2f gridSizeF(static_cast<float>(m_cols), static_cast<float>(m_rows));
    const sf::Vector2f cellSize(windowSizeF.x / gridSizeF.x, windowSizeF.y / gridSizeF.y);

    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            sf::Vertex* triangles = &m_vertices[(r * m_cols + c) * 6];

            sf::Vector2f topLeft(c * cellSize.x, r * cellSize.y);
            sf::Vector2f topRight((c + 1) * cellSize.x, r * cellSize.y);
            sf::Vector2f bottomLeft(c * cellSize.x, (r + 1) * cellSize.y);
            sf::Vector2f bottomRight((c + 1) * cellSize.x, (r + 1) * cellSize.y);

            triangles[0].position = topLeft;
            triangles[1].position = topRight;
            triangles[2].position = bottomLeft;
            triangles[3].position = bottomLeft;
            triangles[4].position = topRight;
            triangles[5].position = bottomRight;

            sf::Color color = (m_grid[r][c] == LIVE) ? sf::Color::White : sf::Color::Black;
            for (int i = 0; i < 6; ++i) {
                triangles[i].color = color;
            }
        }
    }
    m_window.draw(m_vertices);
}