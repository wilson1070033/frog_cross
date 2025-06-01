#include "PowerUp.h"
#include "../core/GameColors.h"

PowerUp::PowerUp(Type t, int posX, int posY, GameColors* gameColors)
    : type(t), x(posX), y(posY), duration(300), active(true), colors(gameColors) {}

bool PowerUp::update() {
    if(active) {
        duration--;
        if(duration <= 0) {
            active = false;
        }
    }
    return active;
}

PowerUp::Type PowerUp::getType() const { return type; }
int PowerUp::getX() const { return x; }
int PowerUp::getY() const { return y; }
bool PowerUp::isActive() const { return active; }

char PowerUp::getSymbol() const {
    switch(type) {
        case SHIELD: return 'S';
        case SPEED_BOOST: return 'B';
        case DOUBLE_SCORE: return 'D';
        case EXTRA_LIFE: return 'L';
        default: return '?';
    }
}

string PowerUp::getColor() const {
    switch(type) {
        case SHIELD: return colors->BRIGHT_YELLOW;
        case SPEED_BOOST: return colors->BRIGHT_MAGENTA;
        case DOUBLE_SCORE: return colors->BRIGHT_CYAN;
        case EXTRA_LIFE: return colors->BRIGHT_GREEN;
        default: return colors->WHITE;
    }
}

string PowerUp::getDescription() const {
    switch(type) {
        case SHIELD: return "Shield - Temporary Protection";
        case SPEED_BOOST: return "Speed Boost - Enhanced Movement";
        case DOUBLE_SCORE: return "Double Score - Bonus Points";
        case EXTRA_LIFE: return "Extra Life - Additional Chance";
        default: return "Unknown Power-up";
    }
}