#include "Car.h"
#include "../core/GameConfig.h"
#include "../core/GameColors.h"
#include "../core/Map.h"
#include <algorithm>

// 建構函數：創建汽車物件，設定顏色為亮青色
Car::Car(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors)
    : Vehicle(x, y, c, d, gameMap, config, gameColors, gameColors->BRIGHT_CYAN) {
    // 從配置中獲取車輛速度，沒有配置時預設為 1.0
    speed = currentConfig ? currentConfig->getVehicleSpeed() : 1.0;
}

// 移動函數：處理汽車的移動邏輯和碰撞檢測
bool Car::move(double deltaTime) {
    moveTimer += deltaTime;  // 累積時間
    double moveInterval = 200.0;  // 移動間隔：200毫秒

    // 檢查是否到了移動時間
    if(moveTimer >= moveInterval) {
        moveTimer = 0.0;  // 重置計時器

        // 計算新位置：當前位置 + (方向 × 速度)
        int newX = x + (d * (int)speed);

        // 邊界處理：車輛從邊界消失後從另一邊出現
        if(newX < 0) newX = map->getWidth() - 1;        // 左邊界換行到右邊
        if(newX >= map->getWidth()) newX = 0;           // 右邊界換行到左邊

        // 碰撞檢測
        bool collision = false;

        // 判斷是否為正常移動（沒有跨越邊界）
        if((x < newX && d > 0) || (x > newX && d < 0)) {
            // 正常移動：檢查從 x 到 newX 之間的路径
            collision = checkPathCollision(x, newX, y);
        } else {
            // 跨邊界移動：分兩段檢查路径
            if(d > 0) {
                // 向右移動：檢查 x 到右邊界 + 左邊界到 newX
                collision = checkPathCollision(x, map->getWidth(), y) ||
                           checkPathCollision(0, newX, y);
            } else {
                // 向左移動：檢查 x 到左邊界 + 右邊界到 newX
                collision = checkPathCollision(x, -1, y) ||
                           checkPathCollision(map->getWidth()-1, newX, y);
            }
        }

        // 如果發生碰撞，返回 false（遊戲結束或失去生命）
        if(collision) {
            return false;
        }

        x = newX;  // 更新位置
    }

    // 在地圖上設定汽車位置並返回成功
    return setXY(x, y, getIcon());
}