#include "GameObject.h"
#include "../core/Map.h"
#include "../core/GameColors.h"
using namespace std;

GameObject::GameObject(int x, int y, char c, Map* gameMap, GameConfig* config, GameColors* gameColors, string col)
    : x(x), y(y), c(c), map(gameMap), currentConfig(config), colors(gameColors), moveTimer(0.0) {
    if(col.empty()) {
        color = colors->WHITE;
    } else {
        color = col;
    }
    setXY(x, y, c);
}

char GameObject::getIcon() { return c; }
string GameObject::getColor() { return color; }
int GameObject::getX() { return x; }
int GameObject::getY() { return y; }

bool GameObject::setXY(int x, int y, char c) {
    if(map) {
        return map->setXY(x, y, c, color);
    }
    return false;
}