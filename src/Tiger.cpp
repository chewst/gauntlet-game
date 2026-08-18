#include "Tiger.hpp"

Tiger::Tiger(int gridX, int gridY, int patrolEndX, Color color, std::string name)
    : Enemy(gridX, gridY, color, std::move(name), /* damage = */ 1),
      m_patrolStartX(gridX),
      m_patrolEndX(patrolEndX),
      m_movingRight(patrolEndX > gridX) {}

void Tiger::takeTurn(const Grid& grid, int playerX, int playerY) {
    // unused for tiger
    (void)playerX;
    (void)playerY;

    const int step = m_movingRight ? 1 : -1;
    const int newX = getX() + step;

    if (grid.isWalkable(newX, getY())) {
        setPosition(newX, getY());
    }

    if (getX() >= m_patrolEndX) {
        m_movingRight = false;
    } else if (getX() <= m_patrolStartX) {
        m_movingRight = true;
    }
}