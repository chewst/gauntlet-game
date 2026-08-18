#pragma once

#include "Enemy.hpp"

// Patrols back and forth between its starting tile and a fixed point
class Tiger : public Enemy {
public:
    Tiger(int gridX, int gridY, int patrolEndX, Color color, std::string name);

    void takeTurn(const Grid& grid, int, int) override;

private:
    int m_patrolStartX;
    int m_patrolEndX;
    bool m_movingRight;
};
