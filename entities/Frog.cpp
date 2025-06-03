#include "Frog.h"
#include "../core/Map.h"
#include "../core/GameConfig.h"
#include "../core/GameColors.h"
#include "../systems/PowerUp.h"
#include <conio.h>

// 建構函數：創建青蛙物件，設定顏色為亮綠色，記錄起始位置
Frog::Frog(int x, int y, char c, Map* gameMap, GameConfig* config, GameColors* gameColors)
    : GameObject(x, y, c, gameMap, config, gameColors, gameColors->BRIGHT_GREEN),
      startX(x), startY(y), hasInputCooldown(false), inputTimer(0.0) {}

// 移動函數：處理玩家輸入、移動邏輯、碰撞檢測和分數計算
bool Frog::move(double deltaTime) {
    // 處理輸入冷卻時間：防止玩家連續快速移動
    if(hasInputCooldown) {
        inputTimer += deltaTime;
        if(inputTimer >= 100.0) {  // 100毫秒冷卻時間
            hasInputCooldown = false;
            inputTimer = 0.0;
        }
    }

    // 檢查是否有按鍵輸入且沒有在冷卻中
    if(kbhit() && !hasInputCooldown) {
        int oldX = x, oldY = y;  // 記錄移動前的位置
        char key = getch();       // 獲取按鍵

        // 處理方向鍵（Windows 系統中方向鍵是雙字元）
        if (key == -32) {
            key = getch();  // 獲取實際的方向鍵代碼
            switch (key) {
                case UP:    // 上移
                    if(y > 0) y--;
                    break;
                case DOWN:  // 下移
                    if(y < map->getHeight()-1) y++;
                    break;
                case LEFT:  // 左移
                    if(x > 0) x--;
                    break;
                case RIGHT: // 右移
                    if(x < map->getWidth()-1) x++;
                    break;
            }
            hasInputCooldown = true;  // 啟動輸入冷卻
        }

        // 障礙物碰撞檢測：如果撞到障礙物，恢復原位置
        if(map->getO(x, y)) {
            x = oldX;
            y = oldY;
        }

        // 道具收集檢測：檢查新位置是否有道具
        PowerUp* powerUp = map->checkPowerUpCollision(x, y);
        if(powerUp) {
            map->collectPowerUp(powerUp);  // 收集道具
        }

        // 分數計算：只有位置真的改變時才計分
        if(x != oldX || y != oldY) {
            int scoreGain = 1;           // 基礎分數

            // 向前移動（向上）獲得更多分數
            if(y < oldY) scoreGain = 3;

            // 在特殊區域移動獲得雙倍分數
            if(map->isScoreZone(x, y)) scoreGain *= 2;

            map->addScore(scoreGain, currentConfig);  // 加分
        }
    }

    // 在地圖上設定青蛙位置
    return setXY(x, y, getIcon());
}

// 重置位置：將青蛙移回起始位置（用於失去生命後）
void Frog::resetPosition() {
    x = startX;
    y = startY;
}

// 檢查是否到達頂部：判斷勝利條件
bool Frog::hasReachedTop() {
    return y == 0;  // 到達地圖頂部（y=0）即為勝利
}