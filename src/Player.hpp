#pragma once

#include "Actor.hpp"

// The character you directly control.
class Player : public Actor {
public:
    Player(int gridX, int gridY, Color color, std::string name);
    char getSymbol() const override;
    
    int getHp() const;
    void takeDamage(int amount);
    bool isAlive() const;

private:
    int m_hp = 3;
};
