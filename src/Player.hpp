#pragma once

#include "Actor.hpp"

// The character player directly control
class Player : public Actor {
public:
    Player(int gridX, int gridY, Color color, std::string name);

    int getHp() const;
    int getMaxHp() const;
    void takeDamage(int amount);
    bool isAlive() const;

    // called when a trapped character joins
    void increaseMaxHp(int amount);

private:
    int m_maxHp = 3;
    int m_hp = 3;
};