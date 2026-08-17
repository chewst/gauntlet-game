#include "Player.hpp"

Player::Player(int gridX, int gridY, Color color, std::string name)
: Actor(gridX, gridY, color, std::move(name)) {}

char Player::getSymbol() const {
    return 'P';
}
