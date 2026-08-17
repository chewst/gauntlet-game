#include "Grid.hpp"

Grid::Grid(int width, int height)
    : m_width(width), m_height(height),
      m_tiles(static_cast<size_t>(height), std::vector<TileType>(static_cast<size_t>(width), TileType::Empty)) {}

int Grid::getWidth() const {
    return m_width;
}

int Grid::getHeight() const {
    return m_height;
}

TileType Grid::getTile(int x, int y) const {
    if (!isInBounds(x, y)) {
        return TileType::Wall;
    }
    return m_tiles[static_cast<size_t>(y)][static_cast<size_t>(x)];
}

void Grid::setTile(int x, int y, TileType type) {
    if (isInBounds(x, y)) {
        m_tiles[static_cast<size_t>(y)][static_cast<size_t>(x)] = type;
    }
}

bool Grid::isInBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < m_width && y < m_height;
}

bool Grid::isWalkable(int x, int y) const {
    return isInBounds(x, y) && getTile(x, y) != TileType::Wall;
}