#include "Frog.h"
#include "../core/Map.h"
#include "../core/GameConfig.h"
#include "../core/GameColors.h"
#include "../systems/PowerUp.h"
#include <conio.h>

Frog::Frog(int x, int y, char c, Map* gameMap, GameConfig* config, GameColors* gameColors)
    : GameObject(x, y, c, gameMap, config, gameColors, gameColors->BRIGHT_GREEN),
      startX(x), startY(y), hasInputCooldown(false), inputTimer(0.0) {}

bool Frog::move(double deltaTime) {
    if(hasInputCooldown) {
        inputTimer += deltaTime;
        if(inputTimer >= 100.0) {
            hasInputCooldown = false;
            inputTimer = 0.0;
        }
    }

    if(kbhit() && !hasInputCooldown) {
        int oldX = x, oldY = y;
        char key = getch();

        if (key == -32) {
            key = getch();
            switch (key) {
                case UP:
                    if(y > 0) y--;
                    break;
                case DOWN:
                    if(y < map->getHeight()-1) y++;
                    break;
                case LEFT:
                    if(x > 0) x--;
                    break;
                case RIGHT:
                    if(x < map->getWidth()-1) x++;
                    break;
            }
            hasInputCooldown = true;
        }

        if(map->getO(x, y)) {
            x = oldX;
            y = oldY;
        }

        PowerUp* powerUp = map->checkPowerUpCollision(x, y);
        if(powerUp) {
            map->collectPowerUp(powerUp);
        }

        if(x != oldX || y != oldY) {
            int scoreGain = 1;
            if(y < oldY) scoreGain = 3;
            if(map->isScoreZone(x, y)) scoreGain *= 2;

            map->addScore(scoreGain, currentConfig);
        }
    }
    return setXY(x, y, getIcon());
}

void Frog::resetPosition() {
    x = startX;
    y = startY;
}

bool Frog::hasReachedTop() {
    return y == 0;
}