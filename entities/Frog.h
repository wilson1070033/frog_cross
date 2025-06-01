#ifndef FROG_H
#define FROG_H

#include "GameObject.h"

// 鍵盤控制常數定義
#define UP    72
#define DOWN  80
#define LEFT  75
#define RIGHT 77

class PowerUp;

class Frog : public GameObject {
private:
    int startX, startY;
    bool hasInputCooldown;
    double inputTimer;

public:
    Frog(int x, int y, char c, Map* gameMap, GameConfig* config, GameColors* gameColors);
    bool move(double deltaTime) override;
    void resetPosition();
    bool hasReachedTop();
};

#endif // FROG_H