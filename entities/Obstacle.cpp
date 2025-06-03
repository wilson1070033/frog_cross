#include "Obstacle.h"
#include "../core/GameColors.h"

// 建構函數：創建障礙物，設定字符為 'O'，顏色為黃色
Obstacle::Obstacle(int x, int y, Map* gameMap, GameConfig* config, GameColors* gameColors)
    : GameObject(x, y, 'O', gameMap, config, gameColors, gameColors->YELLOW) {}

// 移動函數：障礙物是靜態的，不需要移動，始終返回 true
bool Obstacle::move(double deltaTime) {
    return true;  // 靜態物件，不移動，無碰撞風險
}