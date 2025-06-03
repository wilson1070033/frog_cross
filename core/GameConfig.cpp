#include "GameConfig.h"
#include "GameColors.h"
#include <iostream>
using namespace std;

// 建構函數：初始化遊戲的預設配置參數
GameConfig::GameConfig() {
    mapWidth = 40;          // 設定地圖寬度為 40
    mapHeight = 10;         // 設定地圖高度為 10
    gameSpeed = 150;        // 設定遊戲速度（毫秒）
    vehicleSpeed = 1;       // 設定車輛速度倍率
    vehicleCount = 3;       // 設定每條車道的車輛數量
    obstacleCount = 4;      // 設定障礙物總數
    playerLives = 3;        // 設定玩家生命數
    playerLivesMax = 3;     // 紀錄原始生命
    scoreMultiplier = 1.0;  // 設定分數倍率
    vehicleGap = 8;         // 設定車輛間距
    enablePowerUps = true;  // 啟用道具系統
    enableColors = true;    // 啟用彩色顯示
}

// Getter 函數們：取得各種遊戲配置參數的值
int GameConfig::getMapWidth() const { return mapWidth; }
int GameConfig::getMapHeight() const { return mapHeight; }
int GameConfig::getGameSpeed() const { return gameSpeed; }
int GameConfig::getVehicleSpeed() const { return vehicleSpeed; }
int GameConfig::getVehicleCount() const { return vehicleCount; }
int GameConfig::getObstacleCount() const { return obstacleCount; }
int GameConfig::getPlayerLives() const { return playerLives; }
int GameConfig::getPlayerLivesMax() const { return playerLivesMax; }
double GameConfig::getScoreMultiplier() const { return scoreMultiplier; }
int GameConfig::getVehicleGap() const { return vehicleGap; }
bool GameConfig::getPowerUpsEnabled() const { return enablePowerUps; }
bool GameConfig::getColorsEnabled() const { return enableColors; }

// Setter 函數們：設定各種遊戲配置參數，並驗證數值是否在合理範圍內
// 設定地圖寬度，檢查是否在允許範圍內（20-80）
bool GameConfig::setMapWidth(int width) {
    if(width >= limits.minMapWidth && width <= limits.maxMapWidth) {
        mapWidth = width;
        return true; // 設定成功
    }
    return false; // 設定失敗（超出範圍）
}

// 設定地圖高度，檢查是否在允許範圍內（8-20）
bool GameConfig::setMapHeight(int height) {
    if(height >= limits.minMapHeight && height <= limits.maxMapHeight) {
        mapHeight = height;
        return true; // 設定成功
    }
    return false; // 設定失敗（超出範圍）
}

// 設定遊戲速度，檢查是否在允許範圍內（50-500 毫秒）
bool GameConfig::setGameSpeed(int speed) {
    if(speed >= limits.minGameSpeed && speed <= limits.maxGameSpeed) {
        gameSpeed = speed;
        return true;
    }
    return false;
}

// 設定車輛速度倍率，檢查是否在允許範圍內（1-5）
bool GameConfig::setVehicleSpeed(int speed) {
    if(speed >= limits.minVehicleSpeed && speed <= limits.maxVehicleSpeed) {
        vehicleSpeed = speed;
        return true;
    }
    return false;
}

// 設定每條車道的車輛數量，檢查是否在允許範圍內（1-8）
bool GameConfig::setVehicleCount(int count) {
    if(count >= limits.minVehicleCount && count <= limits.maxVehicleCount) {
        vehicleCount = count;
        return true;
    }
    return false;
}

// 設定障礙物總數，檢查是否在允許範圍內（0-15）
bool GameConfig::setObstacleCount(int count) {
    if(count >= limits.minObstacleCount && count <= limits.maxObstacleCount) {
        obstacleCount = count;
        return true;
    }
    return false;
}

// 設定玩家生命數，檢查是否在允許範圍內（1-9）
bool GameConfig::setPlayerLives(int lives) {
    if(lives >= limits.minPlayerLives && lives <= limits.maxPlayerLives) {
        playerLives = lives;
        playerLivesMax = lives;
        return true;
    }
    return false;
}

// 設定分數倍率，檢查是否在允許範圍內（0.5-5.0）
bool GameConfig::setScoreMultiplier(double multiplier) {
    if(multiplier >= limits.minScoreMultiplier && multiplier <= limits.maxScoreMultiplier) {
        scoreMultiplier = multiplier;
        return true;
    }
    return false;
}

// 設定車輛間距，檢查是否在允許範圍內（3-15）
bool GameConfig::setVehicleGap(int gap) {
    if(gap >= limits.minVehicleGap && gap <= limits.maxVehicleGap) {
        vehicleGap = gap;
        return true;
    }
    return false;
}

// 設定是否啟用道具系統（無需驗證，直接設定）
void GameConfig::setPowerUpsEnabled(bool enabled) {
    enablePowerUps = enabled;
}

// 設定是否啟用彩色顯示（無需驗證，直接設定）
void GameConfig::setColorsEnabled(bool enabled) {
    enableColors = enabled;
}

// 顯示當前遊戲配置的詳細資訊，使用彩色輸出格式化顯示
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

// 顯示參數調整指南，提供每個參數的建議範圍和說明
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

// 應用預設配置，根據難度名稱設定對應的遊戲參數組合
void GameConfig::applyPreset(const string& presetName) {
    if(presetName == "CASUAL") {        // 休閒模式：較簡單的配置
        setMapWidth(35);
        setMapHeight(8);
        setGameSpeed(250);              // 較慢的遊戲速度
        setVehicleSpeed(1);
        setVehicleCount(2);             // 較少的車輛
        setObstacleCount(2);            // 較少的障礙物
        setPlayerLives(5);              // 較多的生命
        setScoreMultiplier(0.8);
        setVehicleGap(12);              // 較大的車輛間距
        setPowerUpsEnabled(true);
        setColorsEnabled(true);
    }
    else if(presetName == "CLASSIC") {  // 經典模式：平衡的配置
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
    else if(presetName == "CHALLENGE") { // 挑戰模式：較困難的配置
        setMapWidth(50);
        setMapHeight(12);
        setGameSpeed(100);              // 較快的遊戲速度
        setVehicleSpeed(2);
        setVehicleCount(4);             // 更多的車輛
        setObstacleCount(8);            // 更多的障礙物
        setPlayerLives(2);              // 較少的生命
        setScoreMultiplier(2.0);
        setVehicleGap(5);               // 較小的車輛間距
        setPowerUpsEnabled(false);       // 關閉道具系統
        setColorsEnabled(true);
    }
    else if(presetName == "EXTREME") {   // 極限模式：最困難的配置
        setMapWidth(60);
        setMapHeight(15);
        setGameSpeed(70);               // 最快的遊戲速度
        setVehicleSpeed(3);
        setVehicleCount(6);             // 最多的車輛
        setObstacleCount(12);           // 最多的障礙物
        setPlayerLives(1);              // 只有一條生命
        setScoreMultiplier(3.0);
        setVehicleGap(4);               // 最小的車輛間距
        setPowerUpsEnabled(false);       // 關閉道具系統
        setColorsEnabled(true);
    }
}

// 根據當前配置判斷遊戲難度等級
string GameConfig::getDifficultyName() const {
    if(gameSpeed >= 200 && vehicleSpeed <= 1 && vehicleCount <= 2) return "CASUAL";
    if(gameSpeed >= 150 && vehicleSpeed <= 2 && vehicleCount <= 4) return "CLASSIC";
    if(gameSpeed <= 100 && vehicleSpeed >= 2 && vehicleCount >= 4) return "CHALLENGE";
    if(gameSpeed <= 80 && vehicleSpeed >= 3 && vehicleCount >= 5) return "EXTREME";
    return "CUSTOM";  // 自訂配置，不符合任何預設難度
}