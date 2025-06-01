#include "GameStats.h"
#include "../core/GameColors.h"
#include <iostream>
#include <algorithm>
using namespace std;

GameStats::GameStats(GameColors* gameColors)
    : totalMoves(0), successfulCrossings(0), collisions(0),
      survivalTime(0.0), maxCombo(0), currentCombo(0),
      powerUpsCollected(0), perfectCrossings(0), bestTime(0.0), colors(gameColors) {
    gameStartTime = time(nullptr);
}

void GameStats::recordMove() {
    totalMoves++;
}

void GameStats::recordCrossing() {
    successfulCrossings++;
    currentCombo++;
    maxCombo = max(maxCombo, currentCombo);
    if(collisions == 0) perfectCrossings++;
}

void GameStats::recordCollision() {
    collisions++;
    currentCombo = 0;
}

void GameStats::updateSurvivalTime(double time) {
    survivalTime += time;
    bestTime = max(bestTime, survivalTime);
}

void GameStats::recordPowerUp(PowerUp::Type type) {
    powerUpsCollected++;
    collectedPowerUps.push_back(type);
}

double GameStats::getEfficiencyScore() const {
    if(totalMoves == 0) return 0.0;
    return (double)successfulCrossings / totalMoves * 100.0;
}

void GameStats::displayStats() const {
    cout << colors->BRIGHT_CYAN << "=== ENHANCED GAME STATISTICS ===" << colors->RESET << endl;
    cout << colors->YELLOW << "Total Moves: " << colors->WHITE << totalMoves << endl;
    cout << colors->YELLOW << "Successful Crossings: " << colors->WHITE << successfulCrossings << endl;
    cout << colors->YELLOW << "Perfect Crossings: " << colors->WHITE << perfectCrossings << endl;
    cout << colors->YELLOW << "Collisions: " << colors->WHITE << collisions << endl;
    cout << colors->YELLOW << "Survival Time: " << colors->WHITE << (int)survivalTime / 1000 << "s" << endl;
    cout << colors->YELLOW << "Best Time: " << colors->WHITE << (int)bestTime / 1000 << "s" << endl;
    cout << colors->YELLOW << "Max Combo: " << colors->WHITE << maxCombo << endl;
    cout << colors->YELLOW << "Power-ups Collected: " << colors->WHITE << powerUpsCollected << endl;
    cout << colors->YELLOW << "Efficiency: " << colors->WHITE << (int)getEfficiencyScore() << "%" << endl;
    cout << colors->BRIGHT_CYAN << "=================================" << colors->RESET << endl;
}

int GameStats::getCurrentCombo() const { return currentCombo; }
int GameStats::getPowerUpsCollected() const { return powerUpsCollected; }
double GameStats::getSurvivalTimeSeconds() const { return survivalTime / 1000.0; }
int GameStats::getCollisions() const { return collisions; }