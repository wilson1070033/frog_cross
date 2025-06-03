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

// 建構函數：初始化遊戲管理器，設定隨機種子並初始化所有系統
GameManager::GameManager() {
    srand(static_cast<unsigned int>(time(0)));  // 設定隨機種子，確保每次遊戲的隨機性

    // 初始化所有指標為 nullptr，避免野指標問題
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

    initializeSystems();  // 呼叫系統初始化函數
}

// 解構函數：清理所有系統資源，避免記憶體洩漏
GameManager::~GameManager() {
    cleanupSystems();
}

// 初始化所有遊戲系統：創建核心系統物件和管理系統物件
void GameManager::initializeSystems() {
    // 創建核心系統
    colors = new GameColors();          // 創建顏色管理系統
    utils = new GameUtils();            // 創建工具函數系統
    timer = new GameTimer();            // 創建遊戲計時器
    config = new GameConfig();          // 創建遊戲配置系統
    stats = new GameStats(colors);      // 創建統計追蹤系統
    effects = new EffectSystem(colors); // 創建視覺特效系統

    // 創建管理系統
    dataManager = new DataManager(colors);      // 創建資料管理系統
    dataManager->initialize();                  // 初始化資料管理器

    userMenu = new UserManagementMenu(dataManager, colors);     // 創建用戶管理選單
    configMenu = new ConfigurationMenu(config, colors);        // 創建配置選單
}

// 清理所有系統：刪除動態分配的物件，釋放記憶體
void GameManager::cleanupSystems() {
    cleanupGameObjects();  // 先清理遊戲物件

    // 按照創建的相反順序刪除系統物件
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

// 清理遊戲物件：刪除所有遊戲中的實體物件
void GameManager::cleanupGameObjects() {
    for(auto obj : gameObjects) {
        delete obj;  // 刪除每個遊戲物件
    }
    gameObjects.clear();  // 清空物件容器
    frog = nullptr;       // 重置青蛙指標
}

// 主要遊戲運行函數：管理整個遊戲流程
int GameManager::run() {
    // 顯示用戶管理選單
    if(!showUserManagement()) {
        return 0;  // 如果用戶選擇退出，直接返回
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

// 顯示用戶管理系統：處理登入、註冊等用戶相關功能
bool GameManager::showUserManagement() {
    return userMenu->showUserMenu();
}

// 顯示歡迎畫面：展示遊戲標題、特色功能和當前用戶資訊
void GameManager::showWelcomeScreen() {
    system("cls");  // 清除螢幕
    cout << colors->BRIGHT_CYAN << "\n=====================================================" << colors->RESET << endl;
    cout << colors->BRIGHT_YELLOW << "         WELCOME TO ENHANCED FROG CROSSING!         " << colors->RESET << endl;
    cout << colors->BRIGHT_CYAN << "=====================================================" << colors->RESET << endl;
    cout << colors->GREEN << "A fully customizable and colorful frog crossing experience!" << colors->RESET << endl;
    cout << colors->CYAN << "New features: User profiles, Statistics tracking, Leaderboards!" << colors->RESET << endl;

    // 顯示當前玩家資訊
    if(dataManager->hasLoggedInUser()) {
        cout << colors->BRIGHT_GREEN << "Playing as: " << dataManager->getCurrentUser().username << colors->RESET << endl;
    } else {
        cout << colors->YELLOW << "Playing as: Guest" << colors->RESET << endl;
    }

    cout << colors->BRIGHT_CYAN << "=====================================================" << colors->RESET << endl;
    cout << colors->YELLOW << "\nPress any key to enter enhanced configuration menu..." << colors->RESET << endl;
    getch();  // 等待用戶按鍵
}

// 顯示遊戲配置選單：讓用戶自訂遊戲參數
void GameManager::showGameConfiguration() {
    configMenu->showMainConfigMenu();
}

// 顯示遊戲準備畫面：顯示最終配置和用戶狀態，準備開始遊戲
void GameManager::showGameReadyScreen() {
    system("cls");  // 清除螢幕
    cout << colors->BRIGHT_GREEN << "\n========== READY TO START ENHANCED GAME! ==========" << colors->RESET << endl;
    config->displayCurrentConfig(colors);  // 顯示當前遊戲配置

    // 根據用戶登入狀態顯示不同訊息
    if(dataManager->hasLoggedInUser()) {
        cout << colors->BRIGHT_GREEN << "\nPlaying as registered user: " << dataManager->getCurrentUser().username << colors->RESET << endl;
        cout << colors->CYAN << "Your game progress will be saved automatically!" << colors->RESET << endl;
    } else {
        cout << colors->YELLOW << "\nPlaying as guest - progress will not be saved" << colors->RESET << endl;
        cout << colors->CYAN << "Register an account to track your achievements!" << colors->RESET << endl;
    }

    cout << colors->YELLOW << "\nPress any key to begin your enhanced adventure..." << colors->RESET << endl;
    getch();      // 等待用戶按鍵
    system("cls"); // 清除螢幕準備開始遊戲
}

// 創建遊戲世界：初始化地圖和所有遊戲物件
void GameManager::createGameWorld() {
    // 創建遊戲地圖
    map = new Map(config, colors, utils);
    map->setStats(stats);      // 設定統計系統
    map->setEffects(effects);  // 設定特效系統

    // 生成遊戲物件
    generateFrog();       // 生成青蛙（玩家角色）
    generateObstacles();  // 生成障礙物
    generateVehicles();   // 生成車輛
}

// 生成青蛙：在地圖底部中央位置創建玩家角色
void GameManager::generateFrog() {
    int frogStartX = config->getMapWidth() / 2;     // 青蛙起始 X 座標（地圖中央）
    int frogStartY = config->getMapHeight() - 1;    // 青蛙起始 Y 座標（地圖底部）
    frog = new Frog(frogStartX, frogStartY, 'F', map, config, colors);
    gameObjects.push_back(frog);  // 將青蛙加入遊戲物件清單
}

// 生成障礙物：在地圖上隨機位置創建靜態障礙物
void GameManager::generateObstacles() {
    for(int i = 0; i < config->getObstacleCount(); i++) {
        int ox = utils->randBtw(0, config->getMapWidth() - 1);   // 隨機 X 座標
        int oy = utils->randBtw(1, config->getMapHeight() - 2);  // 隨機 Y 座標（避開邊界）
        gameObjects.push_back(new Obstacle(ox, oy, map, config, colors));
    }
}

// 生成車輛：在每條道路上創建移動的車輛
void GameManager::generateVehicles() {
    for(int i = 0; i < config->getMapHeight(); i++) {
        // 檢查是否為道路且不在邊界
        if(map->isRoad(i) && i > 0 && i < config->getMapHeight() - 1) {
            int direction = (i % 2 == 1) ? 2 : -2;  // 奇數行向右，偶數行向左

            // 生成車輛位置
            vector<int> vehiclePositions = utils->generateVehiclePositions(
                config->getMapWidth(),
                config->getVehicleCount(),
                config->getVehicleGap()
            );

            // 根據道路行數決定車輛類型
            for(int pos : vehiclePositions) {
                if(i % 2 == 0) {
                    // 偶數行生成汽車
                    gameObjects.push_back(new Car(pos, i, 'C', direction, map, config, colors));
                } else {
                    // 奇數行生成機車
                    gameObjects.push_back(new Moto(pos, i, 'M', direction, map, config, colors));
                }
            }
        }
    }
}

// 遊戲主循環：處理遊戲邏輯、更新畫面、檢查碰撞和勝利條件
void GameManager::runGameLoop() {
    while(true) {
        double deltaTime = timer->update();         // 更新時間差
        stats->updateSurvivalTime(deltaTime);      // 更新生存時間統計
        effects->update();                          // 更新視覺特效

        map->fastDraw(dataManager);                 // 快速繪製地圖

        // 檢查勝利條件
        if(frog->hasReachedTop()) {
            handleLevelComplete();  // 處理過關
            break;
        }

        // 更新所有遊戲物件
        for(auto obj : gameObjects) {
            if(!obj->move(deltaTime)) {  // 如果移動失敗（發生碰撞）
                map->loseLife();         // 失去一條生命
                if(map->getLives() <= 0) {
                    handleGameOver();    // 遊戲結束
                    return;
                } else {
                    frog->resetPosition();  // 重置青蛙位置
                    effects->addEffect(config->getMapWidth()/2, config->getMapHeight()/2,
                                    "Life Lost!", colors->BRIGHT_RED, 5);  // 顯示失去生命特效
                    usleep(500 * 1000);  // 暫停 0.5 秒
                }
            }
        }

        usleep(config->getGameSpeed() * 1000);  // 根據遊戲速度暫停
    }
}

// 處理過關：顯示勝利訊息、統計資料並保存遊戲結果
void GameManager::handleLevelComplete() {
    stats->recordCrossing();  // 記錄成功過關
    system("cls");            // 清除螢幕
    cout << colors->BRIGHT_GREEN << "\n\n\n===== LEVEL COMPLETED! =====" << colors->RESET << endl;
    cout << colors->YELLOW << "Congratulations! You reached the stars!" << colors->RESET << endl;
    cout << colors->CYAN << "Your enhanced challenge has been conquered!" << colors->RESET << endl;
    stats->displayStats();   // 顯示遊戲統計

    // 如果是註冊用戶，保存遊戲結果
    if(dataManager->hasLoggedInUser()) {
        dataManager->recordGameResult(
            map->getScore(),                          // 最終分數
            config->getPlayerLives() - map->getLives(), // 使用的生命數
            stats->getSurvivalTimeSeconds(),          // 生存時間
            config->getDifficultyName(),              // 難度名稱
            stats->getPowerUpsCollected()             // 收集的道具數量
        );
        cout << colors->BRIGHT_GREEN << "\nGame result saved to your profile!" << colors->RESET << endl;
    }

    // 倒數計時退出
    for(int countdown = 5; countdown >= 1; countdown--) {
        cout << "\r" << colors->YELLOW << "Game will exit in: " << colors->WHITE << countdown << " " << colors->RESET;
        cout.flush();
        usleep(1000 * 1000);  // 等待 1 秒
    }

    cout << colors->BRIGHT_CYAN << "\n\nThank you for playing the enhanced version!" << colors->RESET << endl;
}

// 處理遊戲結束：顯示失敗訊息、統計資料並保存遊戲結果
void GameManager::handleGameOver() {
    system("cls");  // 清除螢幕
    cout << colors->BRIGHT_RED << "\n\n\n===== GAME OVER =====" << colors->RESET << endl;
    cout << colors->YELLOW << "No more lives remaining!" << colors->RESET << endl;
    cout << colors->CYAN << "Your enhanced configuration provided quite a challenge!" << colors->RESET << endl;
    stats->displayStats();  // 顯示遊戲統計

    // 如果是註冊用戶，保存遊戲結果
    if(dataManager->hasLoggedInUser()) {
        dataManager->recordGameResult(
            map->getScore(),                          // 最終分數
            config->getPlayerLives() - map->getLives(), // 使用的生命數
            stats->getSurvivalTimeSeconds(),          // 生存時間
            config->getDifficultyName(),              // 難度名稱
            stats->getPowerUpsCollected()             // 收集的道具數量
        );
        cout << colors->BRIGHT_GREEN << "\nGame result saved to your profile!" << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Press any key to exit..." << colors->RESET << endl;
    getch();  // 等待用戶按鍵退出
}