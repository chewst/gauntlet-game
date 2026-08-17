#pragma once

#include "raylib.h"
#include <string>

// Base class for anything that lives on the grid
// the player, characters, hazards and animals
// Position is in GRID coordinates (tile x/y)
class Actor {
public:
    Actor(int gridX, int gridY, Color color, std::string name);
    virtual ~Actor() = default;

    int getX() const;
    int getY() const;
    Color getColor() const;
    const std::string& getName() const;

    void setPosition(int gridX, int gridY);

    // Overridden later by things that need custom draw logic
    virtual char getSymbol() const;

private:
    int m_x;
    int m_y;
    Color m_color;
    std::string m_name;
};