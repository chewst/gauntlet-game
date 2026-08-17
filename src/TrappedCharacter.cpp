#include "TrappedCharacter.hpp"

TrappedCharacter::TrappedCharacter(int gridX, int gridY, Color color, std::string name) 
: Actor(gridX, gridY, color, std::move(name)) {}

bool TrappedCharacter::ism_rescued() const {
    return m_rescued;
}

void TrappedCharacter::rescue() {
    m_rescued = true;
}

char TrappedCharacter::getSymbol() const {
    return 'T';
}