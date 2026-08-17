#include "Actor.hpp"

Actor::Actor(int gridX, int gridY, Color color, std::string name)
    : m_x(gridX), m_y(gridY), m_color(color), m_name(std::move(name)) {}

int Actor::getX() const {
    return m_x;
}

int Actor::getY() const {
    return m_y;
}

Color Actor::getColor() const {
    return m_color;
}

const std::string& Actor::getName() const {
    return m_name;
}

void Actor::setPosition(int gridX, int gridY) {
    m_x = gridX;
    m_y = gridY;
}

char Actor::getSymbol() const {
    return '?';
}