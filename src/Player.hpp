#pragma once

#include "Actor.hpp"

// The character you directly control.
class Player : public Actor {
public:
    Player(int gridX, int gridY, Color color, std::string name);
    char getSymbol() const override;
};
