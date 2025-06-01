#include "GameConfig.h"
#include "GameColors.h"
#include <iostream>
using namespace std;

GameConfig::GameConfig() {
    mapWidth = 40;
    mapHeight = 10;
    gameSpeed = 150;
    vehicleSpeed = 1;
    vehicleCount = 3;
    obstacleCount = 4;
    playerLives = 3;
    scoreMultiplier = 1.0;
    vehicleGap = 8;
    enablePowerUps = true;
    enableColors = true;
}

// Getters
int GameConfig::getMapWidth() const { return mapWidth; }
int GameConfig::getMapHeight() const { return mapHeight; }
int GameConfig::getGameSpeed() const { return gameSpeed; }
int GameConfig::getVehicleSpeed() const { return vehicleSpeed; }
int GameConfig::getVehicleCount() const { return vehicleCount; }
int GameConfig::getObstacleCount() const { return obstacleCount; }
int GameConfig::getPlayerLives() const { return playerLives; }
double GameConfig::getScoreMultiplier() const { return scoreMultiplier; }
int GameConfig::getVehicleGap() const { return vehicleGap; }
bool GameConfig::getPowerUpsEnabled() const { return enablePowerUps; }
bool GameConfig::getColorsEnabled() const { return enableColors; }

// Setters
bool GameConfig::setMapWidth(int width) {
    if(width >= limits.minMapWidth && width <= limits.maxMapWidth) {
        mapWidth = width;
        return true;
    }
    return false;
}

bool GameConfig::setMapHeight(int height) {
    if(height >= limits.minMapHeight && height <= limits.maxMapHeight) {
        mapHeight = height;
        return true;
    }
    return false;
}

bool GameConfig::setGameSpeed(int speed) {
    if(speed >= limits.minGameSpeed && speed <= limits.maxGameSpeed) {
        gameSpeed = speed;
        return true;
    }
    return false;
}

bool GameConfig::setVehicleSpeed(int speed) {
    if(speed >= limits.minVehicleSpeed && speed <= limits.maxVehicleSpeed) {
        vehicleSpeed = speed;
        return true;
    }
    return false;
}

bool GameConfig::setVehicleCount(int count) {
    if(count >= limits.minVehicleCount && count <= limits.maxVehicleCount) {
        vehicleCount = count;
        return true;
    }
    return false;
}

bool GameConfig::setObstacleCount(int count) {
    if(count >= limits.minObstacleCount && count <= limits.maxObstacleCount) {
        obstacleCount = count;
        return true;
    }
    return false;
}

bool GameConfig::setPlayerLives(int lives) {
    if(lives >= limits.minPlayerLives && lives <= limits.maxPlayerLives) {
        playerLives = lives;
        return true;
    }
    return false;
}

bool GameConfig::setScoreMultiplier(double multiplier) {
    if(multiplier >= limits.minScoreMultiplier && multiplier <= limits.maxScoreMultiplier) {
        scoreMultiplier = multiplier;
        return true;
    }
    return false;
}

bool GameConfig::setVehicleGap(int gap) {
    if(gap >= limits.minVehicleGap && gap <= limits.maxVehicleGap) {
        vehicleGap = gap;
        return true;
    }
    return false;
}

void GameConfig::setPowerUpsEnabled(bool enabled) {
    enablePowerUps = enabled;
}

void GameConfig::setColorsEnabled(bool enabled) {
    enableColors = enabled;
}

void GameConfig::displayCurrentConfig(GameColors* colors) const {
    cout << colors->BRIGHT_CYAN << "\n========== CURRENT GAME CONFIGURATION ==========" << colors->RESET << endl;
    cout << colors->YELLOW << "Map Size: " << colors->WHITE << mapWidth << " x " << mapHeight << endl;
    cout << colors->YELLOW << "Game Speed: " << colors->WHITE << gameSpeed << " ms (lower = faster)" << endl;
    cout << colors->YELLOW << "Vehicle Speed: " << colors->WHITE << vehicleSpeed << "x" << endl;
    cout << colors->YELLOW << "Vehicles per Lane: " << colors->WHITE << vehicleCount << endl;
    cout << colors->YELLOW << "Total Obstacles: " << colors->WHITE << obstacleCount << endl;
    cout << colors->YELLOW << "Player Lives: " << colors->WHITE << playerLives << endl;
    cout << colors->YELLOW << "Score Multiplier: " << colors->WHITE << scoreMultiplier << "x" << endl;
    cout << colors->YELLOW << "Vehicle Gap: " << colors->WHITE << vehicleGap << " units" << endl;
    cout << colors->YELLOW << "Power-ups: " << colors->WHITE << (enablePowerUps ? "Enabled" : "Disabled") << endl;
    cout << colors->YELLOW << "Colors: " << colors->WHITE << (enableColors ? "Enabled" : "Disabled") << endl;
    cout << colors->BRIGHT_CYAN << "=================================================" << colors->RESET << endl;
}

void GameConfig::displayParameterGuide(GameColors* colors) const {
    cout << colors->BRIGHT_CYAN << "\n============ PARAMETER ADJUSTMENT GUIDE ============" << colors->RESET << endl;
    cout << colors->YELLOW << "Map Width: " << colors->WHITE << limits.minMapWidth << "-" << limits.maxMapWidth
         << " (recommended: 30-50 for balanced gameplay)" << endl;
    cout << colors->YELLOW << "Map Height: " << colors->WHITE << limits.minMapHeight << "-" << limits.maxMapHeight
         << " (recommended: 8-12 for manageable difficulty)" << endl;
    cout << colors->YELLOW << "Game Speed: " << colors->WHITE << limits.minGameSpeed << "-" << limits.maxGameSpeed
         << " ms (lower = faster, 100-200 recommended)" << endl;
    cout << colors->YELLOW << "Vehicle Speed: " << colors->WHITE << limits.minVehicleSpeed << "-" << limits.maxVehicleSpeed
         << "x (1-2 for beginners, 3+ for experts)" << endl;
    cout << colors->YELLOW << "Vehicles per Lane: " << colors->WHITE << limits.minVehicleCount << "-" << limits.maxVehicleCount
         << " (2-4 creates good challenge)" << endl;
    cout << colors->YELLOW << "Obstacles: " << colors->WHITE << limits.minObstacleCount << "-" << limits.maxObstacleCount
         << " (3-6 adds strategic complexity)" << endl;
    cout << colors->YELLOW << "Player Lives: " << colors->WHITE << limits.minPlayerLives << "-" << limits.maxPlayerLives
         << " (3-5 allows learning from mistakes)" << endl;
    cout << colors->YELLOW << "Score Multiplier: " << colors->WHITE << limits.minScoreMultiplier << "-" << limits.maxScoreMultiplier
         << "x (higher rewards skilled play)" << endl;
    cout << colors->YELLOW << "Vehicle Gap: " << colors->WHITE << limits.minVehicleGap << "-" << limits.maxVehicleGap
         << " units (6-10 for comfortable spacing)" << endl;
    cout << colors->BRIGHT_CYAN << "====================================================" << colors->RESET << endl;
}

void GameConfig::applyPreset(const string& presetName) {
    if(presetName == "CASUAL") {
        setMapWidth(35);
        setMapHeight(8);
        setGameSpeed(250);
        setVehicleSpeed(1);
        setVehicleCount(2);
        setObstacleCount(2);
        setPlayerLives(5);
        setScoreMultiplier(0.8);
        setVehicleGap(12);
        setPowerUpsEnabled(true);
        setColorsEnabled(true);
    }
    else if(presetName == "CLASSIC") {
        setMapWidth(40);
        setMapHeight(10);
        setGameSpeed(150);
        setVehicleSpeed(1);
        setVehicleCount(3);
        setObstacleCount(4);
        setPlayerLives(3);
        setScoreMultiplier(1.0);
        setVehicleGap(8);
        setPowerUpsEnabled(true);
        setColorsEnabled(true);
    }
    else if(presetName == "CHALLENGE") {
        setMapWidth(50);
        setMapHeight(12);
        setGameSpeed(100);
        setVehicleSpeed(2);
        setVehicleCount(4);
        setObstacleCount(8);
        setPlayerLives(2);
        setScoreMultiplier(2.0);
        setVehicleGap(5);
        setPowerUpsEnabled(false);
        setColorsEnabled(true);
    }
    else if(presetName == "EXTREME") {
        setMapWidth(60);
        setMapHeight(15);
        setGameSpeed(70);
        setVehicleSpeed(3);
        setVehicleCount(6);
        setObstacleCount(12);
        setPlayerLives(1);
        setScoreMultiplier(3.0);
        setVehicleGap(4);
        setPowerUpsEnabled(false);
        setColorsEnabled(true);
    }
}

string GameConfig::getDifficultyName() const {
    if(gameSpeed >= 200 && vehicleSpeed <= 1 && vehicleCount <= 2) return "CASUAL";
    if(gameSpeed >= 150 && vehicleSpeed <= 2 && vehicleCount <= 4) return "CLASSIC";
    if(gameSpeed <= 100 && vehicleSpeed >= 2 && vehicleCount >= 4) return "CHALLENGE";
    if(gameSpeed <= 80 && vehicleSpeed >= 3 && vehicleCount >= 5) return "EXTREME";
    return "CUSTOM";
}