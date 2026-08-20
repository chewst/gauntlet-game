#pragma once

// A hazard that randomly appears somewhere on the map, stays active for 3 turns and disappears for 1 turn
class Trap {
public:
    Trap(int gridWidth, int gridHeight);

    // when the timer runs out, either hides the trap or re-places it randomly
    void update(const Grid& grid);

    bool isActiveAt(int x, int y) const;
    bool isActive() const;
    int getX() const;
    int getY() const;

private:
    void placeRandomly(const Grid& grid);

    int m_gridWidth;
    int m_gridHeight;
    int m_x;
    int m_y;
    bool m_active;
    int m_turnsRemaining;
};