#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "GameObject.h"

class Obstacle : public GameObject {
public:
    Obstacle(int x, int y, Map* gameMap, GameConfig* config, GameColors* gameColors);
    bool move(double deltaTime) override;
};

#endif // OBSTACLE_H