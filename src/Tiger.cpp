#include "Tiger.hpp"

Tiger::Tiger(int gridX, int gridY, int patrolEnd, Color color, std::string name, bool vertical)
    : Enemy(gridX, gridY, color, std::move(name), /* damage = */ 1),
      m_patrolStart(vertical ? gridY : gridX),
      m_patrolEnd(patrolEnd),
      m_movingForward(patrolEnd > m_patrolStart),
      m_vertical(vertical) {}

void Tiger::takeTurn(const Grid& grid) {

    const int step = m_movingForward ? 1 : -1;

    if (m_vertical) {
        const int newY = getY() + step;
        if (grid.isWalkable(getX(), newY)) {
            setPosition(getX(), newY);
        }
    } else {
        const int newX = getX() + step;
        if (grid.isWalkable(newX, getY())) {
            setPosition(newX, getY());
        }
    }

    const int current = m_vertical ? getY() : getX();
    if (current >= m_patrolEnd) {
        m_movingForward = false;
    } else if (current <= m_patrolStart) {
        m_movingForward = true;
    }
}