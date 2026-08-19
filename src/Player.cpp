#include "Player.hpp"

Player::Player(int gridX, int gridY, Color color, std::string name)
    : Actor(gridX, gridY, color, std::move(name)) {}

int Player::getHp() const {
    return m_hp;
}

int Player::getMaxHp() const {
    return m_maxHp;
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

void Player::increaseMaxHp(int amount) {
    m_maxHp += amount;
    m_hp += amount;
}