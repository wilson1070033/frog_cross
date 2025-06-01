#ifndef MOTO_H
#define MOTO_H

#include "Vehicle.h"

class Moto : public Vehicle {
public:
    Moto(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors);
    bool move(double deltaTime) override;
};

#endif // MOTO_H