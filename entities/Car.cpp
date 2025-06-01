#include "Car.h"
#include "../core/GameConfig.h"
#include "../core/GameColors.h"
#include "../core/Map.h"
#include <algorithm>

Car::Car(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors) 
    : Vehicle(x, y, c, d, gameMap, config, gameColors, gameColors->BRIGHT_CYAN) {
    speed = currentConfig ? currentConfig->getVehicleSpeed() : 1.0;
}

bool Car::move(double deltaTime) {
    moveTimer += deltaTime;
    double moveInterval = 200.0;

    if(moveTimer >= moveInterval) {
        moveTimer = 0.0;

        int newX = x + (d * (int)speed);
        if(newX < 0) newX = map->getWidth() - 1;
        if(newX >= map->getWidth()) newX = 0;

        bool collision = false;
        if((x < newX && d > 0) || (x > newX && d < 0)) {
            collision = checkPathCollision(x, newX, y);
        } else {
            if(d > 0) {
                collision = checkPathCollision(x, map->getWidth(), y) || checkPathCollision(0, newX, y);
            } else {
                collision = checkPathCollision(x, -1, y) || checkPathCollision(map->getWidth()-1, newX, y);
            }
        }

        if(collision) {
            return false;
        }

        x = newX;
    }
    return setXY(x, y, getIcon());
}