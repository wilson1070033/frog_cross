#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>
using namespace std;

// 前向宣告
class GameColors;
class GameUtils;
class GameTimer;
class GameConfig;
class GameStats;
class EffectSystem;
class Map;
class DataManager;
class UserManagementMenu;
class ConfigurationMenu;
class GameObject;
class Frog;

class GameManager {
private:
    // 核心系統
    GameColors* colors;
    GameUtils* utils;
    GameTimer* timer;
    GameConfig* config;
    GameStats* stats;
    EffectSystem* effects;
    Map* map;

    // 管理系統
    DataManager* dataManager;
    UserManagementMenu* userMenu;
    ConfigurationMenu* configMenu;

    // 遊戲物件
    vector<GameObject*> gameObjects;
    Frog* frog;

    // 私有方法
    void initializeSystems();
    void cleanupSystems();
    bool showUserManagement();
    void showWelcomeScreen();
    void showGameConfiguration();
    void showGameReadyScreen();
    void createGameWorld();
    void generateFrog();
    void generateObstacles();
    void generateVehicles();
    void runGameLoop();
    void handleLevelComplete();
    void handleGameOver();
    void cleanupGameObjects();

public:
    GameManager();
    ~GameManager();

    int run();
};

#endif // GAMEMANAGER_H