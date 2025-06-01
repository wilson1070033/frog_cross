#ifndef GAMESTATS_H
#define GAMESTATS_H

#include "PowerUp.h"
#include <vector>
#include <ctime>
using namespace std;

class GameColors;

class GameStats {
private:
    int totalMoves;
    int successfulCrossings;
    int collisions;
    double survivalTime;
    int maxCombo;
    int currentCombo;
    int powerUpsCollected;
    int perfectCrossings;
    double bestTime;
    vector<PowerUp::Type> collectedPowerUps;
    time_t gameStartTime;
    GameColors* colors;

public:
    GameStats(GameColors* gameColors);

    void recordMove();
    void recordCrossing();
    void recordCollision();
    void updateSurvivalTime(double time);
    void recordPowerUp(PowerUp::Type type);

    double getEfficiencyScore() const;
    void displayStats() const;

    int getCurrentCombo() const;
    int getPowerUpsCollected() const;
    double getSurvivalTimeSeconds() const;
    int getCollisions() const;
};

#endif // GAMESTATS_H