#include "Obstacle.h"
#include "../core/GameColors.h"

Obstacle::Obstacle(int x, int y, Map* gameMap, GameConfig* config, GameColors* gameColors)
    : GameObject(x, y, 'O', gameMap, config, gameColors, gameColors->YELLOW) {}

bool Obstacle::move(double deltaTime) {
    return true;
}