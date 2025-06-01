#include "GameManager.h"
#include "GameColors.h"
#include "GameUtils.h"
#include "GameTimer.h"
#include "GameConfig.h"
#include "Map.h"
#include "../systems/GameStats.h"
#include "../systems/EffectSystem.h"
#include "../data/DataManager.h"
#include "../data/UserManagementMenu.h"
#include "../ui/ConfigurationMenu.h"
#include "../entities/GameObject.h"
#include "../entities/Obstacle.h"
#include "../entities/Car.h"
#include "../entities/Moto.h"
#include "../entities/Frog.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <conio.h>
using namespace std;

GameManager::GameManager() {
    srand(static_cast<unsigned int>(time(0)));

    // 初始化所有指針為nullptr
    colors = nullptr;
    utils = nullptr;
    timer = nullptr;
    config = nullptr;
    stats = nullptr;
    effects = nullptr;
    map = nullptr;
    dataManager = nullptr;
    userMenu = nullptr;
    configMenu = nullptr;
    frog = nullptr;

    initializeSystems();
}

GameManager::~GameManager() {
    cleanupSystems();
}

void GameManager::initializeSystems() {
    // 創建核心系統
    colors = new GameColors();
    utils = new GameUtils();
    timer = new GameTimer();
    config = new GameConfig();
    stats = new GameStats(colors);
    effects = new EffectSystem(colors);

    // 創建管理系統
    dataManager = new DataManager(colors);
    dataManager->initialize();

    userMenu = new UserManagementMenu(dataManager, colors);
    configMenu = new ConfigurationMenu(config, colors);
}

void GameManager::cleanupSystems() {
    cleanupGameObjects();

    delete configMenu;
    delete userMenu;
    delete dataManager;
    delete effects;
    delete stats;
    delete map;
    delete config;
    delete timer;
    delete utils;
    delete colors;
}

void GameManager::cleanupGameObjects() {
    for(auto obj : gameObjects) {
        delete obj;
    }
    gameObjects.clear();
    frog = nullptr;
}

int GameManager::run() {
    // 顯示用戶管理選單
    if(!showUserManagement()) {
        return 0;
    }

    // 顯示歡迎畫面
    showWelcomeScreen();

    // 遊戲配置
    showGameConfiguration();

    // 顯示遊戲準備畫面
    showGameReadyScreen();

    // 創建遊戲世界
    createGameWorld();

    // 運行遊戲主循環
    runGameLoop();

    return 0;
}

bool GameManager::showUserManagement() {
    return userMenu->showUserMenu();
}

void GameManager::showWelcomeScreen() {
    system("cls");
    cout << colors->BRIGHT_CYAN << "\n=====================================================" << colors->RESET << endl;
    cout << colors->BRIGHT_YELLOW << "         WELCOME TO ENHANCED FROG CROSSING!         " << colors->RESET << endl;
    cout << colors->BRIGHT_CYAN << "=====================================================" << colors->RESET << endl;
    cout << colors->GREEN << "A fully customizable and colorful frog crossing experience!" << colors->RESET << endl;
    cout << colors->CYAN << "New features: User profiles, Statistics tracking, Leaderboards!" << colors->RESET << endl;

    if(dataManager->hasLoggedInUser()) {
        cout << colors->BRIGHT_GREEN << "Playing as: " << dataManager->getCurrentUser().username << colors->RESET << endl;
    } else {
        cout << colors->YELLOW << "Playing as: Guest" << colors->RESET << endl;
    }

    cout << colors->BRIGHT_CYAN << "=====================================================" << colors->RESET << endl;
    cout << colors->YELLOW << "\nPress any key to enter enhanced configuration menu..." << colors->RESET << endl;
    getch();
}

void GameManager::showGameConfiguration() {
    configMenu->showMainConfigMenu();
}

void GameManager::showGameReadyScreen() {
    system("cls");
    cout << colors->BRIGHT_GREEN << "\n========== READY TO START ENHANCED GAME! ==========" << colors->RESET << endl;
    config->displayCurrentConfig(colors);

    if(dataManager->hasLoggedInUser()) {
        cout << colors->BRIGHT_GREEN << "\nPlaying as registered user: " << dataManager->getCurrentUser().username << colors->RESET << endl;
        cout << colors->CYAN << "Your game progress will be saved automatically!" << colors->RESET << endl;
    } else {
        cout << colors->YELLOW << "\nPlaying as guest - progress will not be saved" << colors->RESET << endl;
        cout << colors->CYAN << "Register an account to track your achievements!" << colors->RESET << endl;
    }

    cout << colors->YELLOW << "\nPress any key to begin your enhanced adventure..." << colors->RESET << endl;
    getch();
    system("cls");
}

void GameManager::createGameWorld() {
    // 創建遊戲地圖
    map = new Map(config, colors, utils);
    map->setStats(stats);
    map->setEffects(effects);

    // 生成遊戲物件
    generateFrog();
    generateObstacles();
    generateVehicles();
}

void GameManager::generateFrog() {
    int frogStartX = config->getMapWidth() / 2;
    int frogStartY = config->getMapHeight() - 1;
    frog = new Frog(frogStartX, frogStartY, 'F', map, config, colors);
    gameObjects.push_back(frog);
}

void GameManager::generateObstacles() {
    for(int i = 0; i < config->getObstacleCount(); i++) {
        int ox = utils->randBtw(0, config->getMapWidth() - 1);
        int oy = utils->randBtw(1, config->getMapHeight() - 2);
        gameObjects.push_back(new Obstacle(ox, oy, map, config, colors));
    }
}

void GameManager::generateVehicles() {
    for(int i = 0; i < config->getMapHeight(); i++) {
        if(map->isRoad(i) && i > 0 && i < config->getMapHeight() - 1) {
            int direction = (i % 2 == 1) ? 2 : -2;

            vector<int> vehiclePositions = utils->generateVehiclePositions(
                config->getMapWidth(),
                config->getVehicleCount(),
                config->getVehicleGap()
            );

            for(int pos : vehiclePositions) {
                if(i % 2 == 0) {
                    gameObjects.push_back(new Car(pos, i, 'C', direction, map, config, colors));
                } else {
                    gameObjects.push_back(new Moto(pos, i, 'M', direction, map, config, colors));
                }
            }
        }
    }
}

void GameManager::runGameLoop() {
    while(true) {
        double deltaTime = timer->update();
        stats->updateSurvivalTime(deltaTime);
        effects->update();

        map->fastDraw(dataManager);

        // 檢查勝利條件
        if(frog->hasReachedTop()) {
            handleLevelComplete();
            break;
        }

        // 更新所有遊戲物件
        for(auto obj : gameObjects) {
            if(!obj->move(deltaTime)) {
                map->loseLife();
                if(map->getLives() <= 0) {
                    handleGameOver();
                    return;
                } else {
                    frog->resetPosition();
                    effects->addEffect(config->getMapWidth()/2, config->getMapHeight()/2,
                                    "Life Lost!", colors->BRIGHT_RED, 5);
                    usleep(500 * 1000);
                }
            }
        }

        usleep(config->getGameSpeed() * 1000);
    }
}

void GameManager::handleLevelComplete() {
    stats->recordCrossing();
    system("cls");
    cout << colors->BRIGHT_GREEN << "\n\n\n===== LEVEL COMPLETED! =====" << colors->RESET << endl;
    cout << colors->YELLOW << "Congratulations! You reached the stars!" << colors->RESET << endl;
    cout << colors->CYAN << "Your enhanced challenge has been conquered!" << colors->RESET << endl;
    stats->displayStats();

    if(dataManager->hasLoggedInUser()) {
        dataManager->recordGameResult(
            map->getScore(),
            config->getPlayerLives() - map->getLives(),
            stats->getSurvivalTimeSeconds(),
            config->getDifficultyName(),
            stats->getPowerUpsCollected()
        );
        cout << colors->BRIGHT_GREEN << "\nGame result saved to your profile!" << colors->RESET << endl;
    }

    for(int countdown = 5; countdown >= 1; countdown--) {
        cout << "\r" << colors->YELLOW << "Game will exit in: " << colors->WHITE << countdown << " " << colors->RESET;
        cout.flush();
        usleep(1000 * 1000);
    }

    cout << colors->BRIGHT_CYAN << "\n\nThank you for playing the enhanced version!" << colors->RESET << endl;
}

void GameManager::handleGameOver() {
    system("cls");
    cout << colors->BRIGHT_RED << "\n\n\n===== GAME OVER =====" << colors->RESET << endl;
    cout << colors->YELLOW << "No more lives remaining!" << colors->RESET << endl;
    cout << colors->CYAN << "Your enhanced configuration provided quite a challenge!" << colors->RESET << endl;
    stats->displayStats();

    if(dataManager->hasLoggedInUser()) {
        dataManager->recordGameResult(
            map->getScore(),
            config->getPlayerLives() - map->getLives(),
            stats->getSurvivalTimeSeconds(),
            config->getDifficultyName(),
            stats->getPowerUpsCollected()
        );
        cout << colors->BRIGHT_GREEN << "\nGame result saved to your profile!" << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Press any key to exit..." << colors->RESET << endl;
    getch();
}