#include "Map.h"
#include "GameColors.h"
#include "GameUtils.h"
#include "GameConfig.h"
#include "../systems/GameStats.h"
#include "../systems/EffectSystem.h"
#include "../systems/PowerUp.h"
#include "../data/DataManager.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ColoredChar 結構體實作
// 建構函數：設定字符和顏色
ColoredChar::ColoredChar(char c, string col) : character(c), color(col) {}

// 轉換為帶顏色的字串輸出
string ColoredChar::toString() const {
    return color + character + "\033[0m";
}

// Map 類別實作
// 建構函數：初始化地圖參數和生成地圖
Map::Map(GameConfig* gameConfig, GameColors* gameColors, GameUtils* gameUtils)
    : config(gameConfig), colors(gameColors), utils(gameUtils), stats(nullptr), effects(nullptr) {
    W = config->getMapWidth();        // 設定地圖寬度
    H = config->getMapHeight();       // 設定地圖高度
    lives = config->getPlayerLives(); // 設定玩家生命數
    livesMax = config->getPlayerLivesMax();
    R = new int[H];                   // 分配道路陣列記憶體
    generateRoadPattern();            // 生成道路模式
    initializeMap();                  // 初始化地圖內容
}

// 解構函數：清理記憶體
Map::~Map() {
    delete[] R;  // 釋放道路陣列
    for(auto powerUp : powerUps) {
        delete powerUp;  // 刪除所有道具
    }
}

// 生成道路模式：決定哪些行是道路(0)或安全區(1)
void Map::generateRoadPattern() {
    R[0] = 1;    // 頂部是安全區
    R[H-1] = 1;  // 底部是安全區

    // 設定中間行的道路模式
    for(int i = 1; i < H-1; i++) {
        if(i == H/2) {
            R[i] = 1;  // 中間設一個安全區
        } else {
            R[i] = 0;  // 其他設為道路
        }
    }

    // 檢查道路數量，確保至少有2條道路
    int roadCount = 0;
    for(int i = 0; i < H; i++) {
        if(R[i] == 0) roadCount++;
    }

    // 如果道路太少，重新設定為奇數行都是道路
    if(roadCount < 2) {
        for(int i = 1; i < H-1; i += 2) {
            R[i] = 0;
        }
    }
}

// 初始化地圖內容：設定基本地形、道具和標記
void Map::initializeMap() {
    // 創建空白地圖網格
    grid = vector<vector<ColoredChar>>(H, vector<ColoredChar>(W, ColoredChar('.', colors->WHITE)));

    // 填入安全區域(=)
    for(int i = 0; i < H; i++) {
        if(R[i] == 1) {
            for(int j = 0; j < W; j++) {
                grid[i][j] = ColoredChar('=', colors->BRIGHT_YELLOW);
            }
        }
    }

    g0 = grid;  // 保存原始地圖狀態

    // 如果啟用道具系統，生成道具
    if(config && config->getPowerUpsEnabled()) {
        generatePowerUps();
    }

    // 在道路兩端放置車輛指示器
    for(int i = 0; i < H; i++) {
        if(R[i] == 0) {
            if(i % 2 == 1) {
                grid[i][0] = ColoredChar('C', colors->BRIGHT_CYAN);      // 奇數行左側
            } else {
                grid[i][W - 1] = ColoredChar('C', colors->BRIGHT_CYAN); // 偶數行右側
            }
        }
    }

    // 放置目標星號(*)和起始波浪(~)
    for(int i = 0; i < W; i += max(6, W/8)) {
        if(i < W) {
            grid[0][i] = ColoredChar('*', colors->BRIGHT_YELLOW);        // 頂部目標
            if(H > 1) grid[H-1][i] = ColoredChar('~', colors->BRIGHT_BLUE); // 底部起點
        }
    }
}

// 生成道具：在安全區域隨機放置道具
void Map::generatePowerUps() {
    int powerUpCount = max(1, W / 15);  // 根據地圖寬度決定道具數量

    for(int i = 0; i < powerUpCount; i++) {
        PowerUp::Type type = static_cast<PowerUp::Type>(rand() % 4);  // 隨機道具類型
        int x = utils->randBtw(1, W - 2);  // 隨機 X 座標
        int y = utils->randBtw(1, H - 2);  // 隨機 Y 座標

        // 只在安全區域放置道具
        if(R[y] == 1) {
            powerUps.push_back(new PowerUp(type, x, y, colors));
        }
    }
}

// 更新道具狀態：移除過期道具，繪製活躍道具
void Map::updatePowerUps() {
    // 移除已失效的道具
    powerUps.erase(
        remove_if(powerUps.begin(), powerUps.end(),
            [](PowerUp* p) {
                if(!p->update()) {
                    delete p;
                    return true;
                }
                return false;
            }),
        powerUps.end()
    );

    // 繪製所有活躍道具
    for(auto powerUp : powerUps) {
        if(powerUp->isActive()) {
            setXY(powerUp->getX(), powerUp->getY(), powerUp->getSymbol(), powerUp->getColor());
        }
    }
}

// 快速繪製地圖：清除螢幕並重新繪製整個遊戲畫面
void Map::fastDraw(DataManager* dataManager) {
    // 跨平台清屏
    #ifdef _WIN32
        system("cls");
    #else
        cout << "\033[2J\033[H";
    #endif

    // 顯示遊戲標題
    cout << colors->BRIGHT_CYAN << "===== ENHANCED FROG CROSSING GAME =====" << colors->RESET << endl;

    // 顯示用戶統計摘要
    if(dataManager->hasLoggedInUser()) {
        cout << colors->BRIGHT_GREEN << dataManager->getUserStatsSummary() << colors->RESET << endl;
    }

    // 顯示分數和生命值
    cout << colors->YELLOW << "Score: " << colors->WHITE << score << colors->YELLOW << " | Lives: " << colors->RESET;

    // 繪製生命值圖示
    for(int i = 0; i < lives; i++) {
        cout << colors->BRIGHT_GREEN << "[*]" << colors->RESET;
    }
    for(int i = lives; i < livesMax; i++) {
        cout << colors->RED << "[ ]" << colors->RESET;
    }

    // 顯示統計資訊
    if(stats) {
        cout << colors->YELLOW << " | Combo: " << colors->WHITE << stats->getCurrentCombo();
        cout << colors->YELLOW << " | Efficiency: " << colors->WHITE << (int)stats->getEfficiencyScore() << "%";
        cout << colors->YELLOW << " | Power-ups: " << colors->WHITE << stats->getPowerUpsCollected();
    }
    cout << endl;

    // 顯示控制說明
    cout << colors->CYAN << "Controls: Arrow Keys | Goal: Reach the " << colors->BRIGHT_YELLOW << "stars" << colors->CYAN << "!" << colors->RESET << endl;

    // 繪製頂部邊界
    for(int i = 0; i < W; i++) cout << colors->BRIGHT_BLUE << "=" << colors->RESET;
    cout << endl;

    // 繪製地圖內容
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            if(config && config->getColorsEnabled()) {
                cout << grid[i][j].toString();  // 彩色輸出
            } else {
                cout << grid[i][j].character;   // 純文字輸出
            }
        }
        cout << endl;
    }

    // 繪製底部邊界
    for(int i = 0; i < W; i++) cout << colors->BRIGHT_BLUE << "=" << colors->RESET;
    cout << endl;

    // 顯示特殊狀態資訊
    if(effects && effects->hasActiveEffects()) {
        cout << colors->BRIGHT_MAGENTA << "*** SPECIAL EFFECTS ACTIVE ***" << colors->RESET << endl;
    }

    if(config && config->getPowerUpsEnabled() && !powerUps.empty()) {
        cout << colors->BRIGHT_CYAN << "Active Power-ups: " << powerUps.size() << colors->RESET << endl;
    }

    grid = g0;        // 重置地圖為原始狀態
    updatePowerUps(); // 更新道具
}

// 設定指定座標的字符和顏色，並檢查碰撞
bool Map::setXY(int x, int y, char c, string color) {
    // 邊界檢查
    if(y >= 0 && y < H && x >= 0 && x < W) {
        // 如果沒指定顏色，根據字符類型設定預設顏色
        if(color.empty()) {
            switch(c) {
                case 'F': color = colors->BRIGHT_GREEN; break;  // 青蛙
                case 'C': color = colors->BRIGHT_CYAN; break;   // 汽車
                case 'M': color = colors->BRIGHT_RED; break;    // 機車
                case 'O': color = colors->YELLOW; break;        // 障礙物
                default:  color = colors->WHITE; break;
            }
        }

        // 障礙物設定到原始地圖
        if(c == 'O') g0[y][x] = ColoredChar(c, color);

        // 碰撞檢測：青蛙與車輛相撞
        if(c == 'F' && grid[y][x].character == 'C') return false;
        if(c == 'F' && grid[y][x].character == 'M') return false;
        if(c == 'C' && grid[y][x].character == 'F') return false;
        if(c == 'M' && grid[y][x].character == 'F') return false;

        grid[y][x] = ColoredChar(c, color);
    }
    return true;
}

// 檢查指定位置是否有道具碰撞
PowerUp* Map::checkPowerUpCollision(int x, int y) {
    for(auto powerUp : powerUps) {
        if(powerUp->isActive() && powerUp->getX() == x && powerUp->getY() == y) {
            return powerUp;
        }
    }
    return nullptr;
}

// 收集道具：記錄統計、顯示特效、應用道具效果
void Map::collectPowerUp(PowerUp* powerUp) {
    // 記錄統計
    if(stats) {
        stats->recordPowerUp(powerUp->getType());
    }

    // 顯示收集特效
    if(effects) {
        effects->addEffect(powerUp->getX(), powerUp->getY(),
                         powerUp->getDescription(),
                         powerUp->getColor(), 7);
    }

    // 應用道具效果
    switch(powerUp->getType()) {
        case PowerUp::SHIELD:
            // 加入護盾效果的處理邏輯
            break;
        case PowerUp::SPEED_BOOST:
            // 加入速度提升效果的處理邏輯
            break;
        case PowerUp::EXTRA_LIFE:
            lives++;        // 增加生命
            break;
        case PowerUp::DOUBLE_SCORE:
            score += 50;    // 增加分數
            break;
    }

    powerUp->update();  // 標記道具為已使用
}

// 增加分數：應用分數倍率並記錄統計
void Map::addScore(int i, GameConfig* config) {
    double multiplier = config ? config->getScoreMultiplier() : 1.0;
    int actualScore = (int)(i * multiplier);
    score += actualScore;

    if(stats) {
        stats->recordMove();  // 記錄移動
        if(effects && actualScore > 1) {
            // 顯示得分特效
            effects->addEffect(W/2, H/2, "+" + to_string(actualScore), colors->BRIGHT_GREEN, 3);
        }
    }
}

// 失去生命：減少生命數並記錄統計
void Map::loseLife() {
    if(lives > 0) {
        lives--;
        if(stats) stats->recordCollision();  // 記錄碰撞
        if(effects) effects->addEffect(W/2, H/2, "COLLISION!", colors->BRIGHT_RED, 5);
    }
}

// Getter 函數群
int Map::getLives() const { return lives; }
int Map::getScore() const { return score; }
int Map::getWidth() const { return W; }
int Map::getHeight() const { return H; }

// 檢查指定位置是否有障礙物
bool Map::getO(int x, int y) {
    if(y >= 0 && y < H && x >= 0 && x < W) {
        if(g0[y][x].character == 'O') return true;
    }
    return false;
}

// 重置青蛙：清除地圖上的青蛙字符
void Map::resetFrog(int x, int y) {
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            if(grid[i][j].character == 'F') {
                grid[i][j] = g0[i][j];  // 恢復原始地圖內容
            }
        }
    }
}

// 獲取指定位置的字符
char Map::getCharAt(int x, int y) {
    if(y >= 0 && y < H && x >= 0 && x < W) {
        return grid[y][x].character;
    }
    return '.';
}

// 設定系統指標
void Map::setStats(GameStats* s) { stats = s; }
void Map::setEffects(EffectSystem* e) { effects = e; }

// 檢查是否為高分區域
bool Map::isScoreZone(int x, int y) {
    return (y <= 2 && g0[y][x].character == '.');
}

// 檢查指定行是否為道路
bool Map::isRoad(int y) {
    if(y >= 0 && y < H) {
        return R[y] == 0;  // 0表示道路，1表示安全區
    }
    return false;
}