#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
using namespace std;

// 前向宣告
class GameColors;
class GameUtils;
class GameConfig;
class GameStats;
class EffectSystem;
class PowerUp;
class DataManager;

// Enhanced colored character structure
struct ColoredChar {
    char character;
    string color;

    ColoredChar(char c = '.', string col = "");
    string toString() const;
};

class Map {
private:
    int W, H;
    int* R;
    int score = 0;
    int lives;
    int livesMax;
    vector<vector<ColoredChar>> grid, g0;
    vector<PowerUp*> powerUps;
    GameConfig* config;
    GameColors* colors;
    GameUtils* utils;
    GameStats* stats;
    EffectSystem* effects;

    void generateRoadPattern();
    void initializeMap();
    void generatePowerUps();
    void updatePowerUps();

public:
    Map(GameConfig* gameConfig, GameColors* gameColors, GameUtils* gameUtils);
    ~Map();

    void fastDraw(DataManager* dataManager);
    bool setXY(int x, int y, char c, string color = "");
    PowerUp* checkPowerUpCollision(int x, int y);
    void collectPowerUp(PowerUp* powerUp);
    void addScore(int i, GameConfig* config = nullptr);
    void loseLife();

    // Getters
    int getLives() const;
    int getScore() const;
    int getWidth() const;
    int getHeight() const;
    char getCharAt(int x, int y);
    bool getO(int x, int y);
    bool isScoreZone(int x, int y);
    bool isRoad(int y);

    void resetFrog(int x, int y);
    void setStats(GameStats* s);
    void setEffects(EffectSystem* e);
};

#endif // MAP_H