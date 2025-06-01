#include "Vehicle.h"
#include "../core/Map.h"

Vehicle::Vehicle(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors, string col)
    : GameObject(x, y, c, gameMap, config, gameColors, col), d(d), speed(1.0) {}

bool Vehicle::checkPathCollision(int startX, int endX, int y) {
    if(startX == endX) return false;
    int direction = (endX > startX) ? 1 : -1;
    for(int x = startX; x != endX; x += direction) {
        if(x >= 0 && x < map->getWidth() && y >= 0 && y < map->getHeight()) {
            if(map && map->getCharAt(x, y) == 'F') {
                return true;
            }
        }
    }
    return false;
}