#include "PowerUp.h"
#include "../core/GameColors.h"

// 建構函數：創建道具，設定類型、位置和初始持續時間（300）
PowerUp::PowerUp(Type t, int posX, int posY, GameColors* gameColors)
    : type(t), x(posX), y(posY), duration(300), active(true), colors(gameColors) {}

// 更新道具狀態：減少持續時間，時間歸零時設為非活躍
bool PowerUp::update() {
    if(active) {
        duration--;               // 持續時間遞減
        if(duration <= 0) {
            active = false;       // 時間用完，道具失效
        }
    }
    return active;                // 返回道具是否仍然活躍
}

// Getter 函數群：返回道具的基本屬性
PowerUp::Type PowerUp::getType() const { return type; }    // 獲取道具類型
int PowerUp::getX() const { return x; }                    // 獲取 X 座標
int PowerUp::getY() const { return y; }                    // 獲取 Y 座標
bool PowerUp::isActive() const { return active; }          // 檢查是否活躍

// 獲取道具顯示字符：根據道具類型返回對應的字母
char PowerUp::getSymbol() const {
    switch(type) {
        case SHIELD: return 'S';        // 護盾
        case SPEED_BOOST: return 'B';   // 速度提升 (Boost)
        case DOUBLE_SCORE: return 'D';  // 雙倍分數 (Double)
        case EXTRA_LIFE: return 'L';    // 額外生命 (Life)
        default: return '?';            // 未知道具
    }
}

// 獲取道具顏色：根據道具類型返回對應的亮色
string PowerUp::getColor() const {
    switch(type) {
        case SHIELD: return colors->BRIGHT_YELLOW;     // 護盾：亮黃色
        case SPEED_BOOST: return colors->BRIGHT_MAGENTA; // 速度：亮紫色
        case DOUBLE_SCORE: return colors->BRIGHT_CYAN;   // 分數：亮青色
        case EXTRA_LIFE: return colors->BRIGHT_GREEN;    // 生命：亮綠色
        default: return colors->WHITE;                   // 預設：白色
    }
}

// 獲取道具描述：返回道具的詳細說明文字
string PowerUp::getDescription() const {
    switch(type) {
        case SHIELD: return "Shield - Temporary Protection";        // 護盾：暫時保護
        case SPEED_BOOST: return "Speed Boost - Enhanced Movement"; // 速度：增強移動
        case DOUBLE_SCORE: return "Double Score - Bonus Points";    // 分數：獲得獎勵分數
        case EXTRA_LIFE: return "Extra Life - Additional Chance";   // 生命：額外機會
        default: return "Unknown Power-up";                         // 未知道具
    }
}