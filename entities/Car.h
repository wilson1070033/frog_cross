#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle {
public:
    Car(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors);
    bool move(double deltaTime) override;
};

#endif // CAR_H