#include "Trap.hpp"
#include <cstdlib>

const int ACTIVE_DURATION = 5;  
const int HIDDEN_DURATION = 3;   // turns the trap stays hidden before reappearing

Trap::Trap(int gridWidth, int gridHeight) : m_gridWidth(gridWidth), m_gridHeight(gridHeight),
    m_x(0), m_y(0), m_active(false), m_turnsRemaining(0) {}

void Trap::placeRandomly(const Grid& grid) {
    // Pick random tiles until we find an open floor tile (not a wall, hazard, or exit)
    // A cap on attempts avoids looping forever if the map is somehow entirely full
    const int maxAttempts = 100;
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        const int candidateX = (std::rand() % (m_gridWidth - 2)) + 1;
        const int candidateY = (std::rand() % (m_gridHeight - 2)) + 1;

        if (grid.getTile(candidateX, candidateY) == TileType::Empty) {
            m_x = candidateX;
            m_y = candidateY;
            return;
        }
    }
}

void Trap::update(const Grid& grid) {
    --m_turnsRemaining;

    if (m_turnsRemaining > 0) {
        return;  // still in the current state, so nothing changes yet
    }

    if (m_active) {
        m_active = false;
        m_turnsRemaining = HIDDEN_DURATION;
    } else {
        placeRandomly(grid);
        m_active = true;
        m_turnsRemaining = ACTIVE_DURATION;
    }
}

bool Trap::isActiveAt(int x, int y) const {
    return m_active && m_x == x && m_y == y;
}

bool Trap::isActive() const {
    return m_active;
}

int Trap::getX() const {
    return m_x;
}

int Trap::getY() const {
    return m_y;
}