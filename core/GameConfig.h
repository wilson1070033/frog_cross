#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <string>
using namespace std;

// 前向宣告
class GameColors;

class GameConfig {
private:
    int mapWidth;
    int mapHeight;
    int gameSpeed;
    int vehicleSpeed;
    int vehicleCount;
    int obstacleCount;
    int playerLives;
    int playerLivesMax;
    double scoreMultiplier;
    int vehicleGap;
    bool enablePowerUps;
    bool enableColors;

    struct ParameterLimits {
        int minMapWidth = 20, maxMapWidth = 80;
        int minMapHeight = 8, maxMapHeight = 20;
        int minGameSpeed = 50, maxGameSpeed = 500;
        int minVehicleSpeed = 1, maxVehicleSpeed = 5;
        int minVehicleCount = 1, maxVehicleCount = 8;
        int minObstacleCount = 0, maxObstacleCount = 15;
        int minPlayerLives = 1, maxPlayerLives = 9;
        double minScoreMultiplier = 0.5, maxScoreMultiplier = 5.0;
        int minVehicleGap = 3, maxVehicleGap = 15;
    } limits;

public:
    GameConfig();
    
    // Getters
    int getMapWidth() const;
    int getMapHeight() const;
    int getGameSpeed() const;
    int getVehicleSpeed() const;
    int getVehicleCount() const;
    int getObstacleCount() const;
    int getPlayerLives() const;
    int getPlayerLivesMax() const;
    double getScoreMultiplier() const;
    int getVehicleGap() const;
    bool getPowerUpsEnabled() const;
    bool getColorsEnabled() const;

    // Setters
    bool setMapWidth(int width);
    bool setMapHeight(int height);
    bool setGameSpeed(int speed);
    bool setVehicleSpeed(int speed);
    bool setVehicleCount(int count);
    bool setObstacleCount(int count);
    bool setPlayerLives(int lives);
    bool setScoreMultiplier(double multiplier);
    bool setVehicleGap(int gap);
    void setPowerUpsEnabled(bool enabled);
    void setColorsEnabled(bool enabled);

    // 其他方法
    void displayCurrentConfig(GameColors* colors) const;
    void displayParameterGuide(GameColors* colors) const;
    void applyPreset(const string& presetName);
    string getDifficultyName() const;
};

#endif // GAMECONFIG_H