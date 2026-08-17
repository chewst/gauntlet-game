#pragma once

#include <vector>

enum class TileType {
    Empty,
    Wall,
    Hazard,
    Exit
};

// Owns the tile layout for one map
class Grid {
public:
    Grid(int width, int height);

    int getWidth() const;
    int getHeight() const;

    TileType getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);

    bool isInBounds(int x, int y) const;
    bool isWalkable(int x, int y) const;

private:
    int m_width;
    int m_height;
    std::vector<std::vector<TileType>> m_tiles;
};