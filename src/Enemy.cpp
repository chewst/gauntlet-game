#include "Enemy.hpp"

Enemy::Enemy(int gridX, int gridY, Color color, std::string name, int damage)
    : Actor(gridX, gridY, color, std::move(name)), m_damage(damage) {}

int Enemy::getDamage() const {
    return m_damage;
}