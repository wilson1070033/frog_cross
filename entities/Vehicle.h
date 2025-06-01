#ifndef VEHICLE_H
#define VEHICLE_H

#include "GameObject.h"

class Vehicle : public GameObject {
protected:
    int d;
    double speed;

    bool checkPathCollision(int startX, int endX, int y);

public:
    Vehicle(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors, string col);
    virtual bool move(double deltaTime) = 0;
};

#endif // VEHICLE_H