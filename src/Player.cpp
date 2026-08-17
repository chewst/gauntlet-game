#include "Player.hpp"

Player::Player(int gridX, int gridY, Color color, std::string name)
: Actor(gridX, gridY, color, std::move(name)) {}

char Player::getSymbol() const {
    return 'P';
}

int Player::getHp() const {
    return m_hp;
}

void Player::takeDamage(int amount) {
    m_hp -= amount;
    if (m_hp < 0) {
        m_hp = 0;
    }
}

bool Player::isAlive() const {
    return m_hp > 0;
}
