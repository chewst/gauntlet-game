#pragma once

#include "Actor.hpp"

// A trapped character sitting still until rescued 
class TrappedCharacter : public Actor {
public:
    TrappedCharacter(int gridX, int gridY, Color color, std::string name);

    bool isRescued() const;
    void rescue();

    char getSymbol() const override;

private:
    bool m_rescued = false;
};