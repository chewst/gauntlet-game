#pragma once

#include "Enemy.hpp"

// Patrols back and forth between its starting tile and a fixed point
class Tiger : public Enemy {
public:
    Tiger(int gridX, int gridY, int patrolEnd, Color color, std::string name, bool vertical = false);

    void takeTurn(const Grid& grid, int playerX, int playerY) override;

private:
    int m_patrolStart;
    int m_patrolEnd;
    bool m_movingForward;
    bool m_vertical;
};