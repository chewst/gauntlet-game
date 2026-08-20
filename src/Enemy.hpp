#pragma once
#include "Actor.hpp"

// Base class for enemies that moves on its own each turn
class Enemy : public Actor {
public:
    Enemy(int gridX, int gridY, Color color, std::string name, int damage);

    // takeTurn() called once per player move
    virtual void takeTurn(const Grid& grid) = 0;

    int getDamage() const;

private:
    int m_damage;
};