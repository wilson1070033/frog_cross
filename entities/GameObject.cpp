#include "GameObject.h"
#include "../core/Map.h"
#include "../core/GameColors.h"
using namespace std;

// 建構函數：初始化遊戲物件的基本屬性
GameObject::GameObject(int x, int y, char c, Map* gameMap, GameConfig* config, GameColors* gameColors, string col)
    : x(x), y(y), c(c), map(gameMap), currentConfig(config), colors(gameColors), moveTimer(0.0) {

    // 設定物件顏色：如果沒有指定顏色則使用白色
    if(col.empty()) {
        color = colors->WHITE;
    } else {
        color = col;
    }

    // 在地圖上設定物件的初始位置
    setXY(x, y, c);
}

// Getter 函數群：返回物件的基本屬性
char GameObject::getIcon() { return c; }        // 獲取物件圖示字符
string GameObject::getColor() { return color; } // 獲取物件顏色
int GameObject::getX() { return x; }            // 獲取 X 座標
int GameObject::getY() { return y; }            // 獲取 Y 座標

// 設定物件在地圖上的位置：委託給地圖物件處理
bool GameObject::setXY(int x, int y, char c) {
    if(map) {
        // 呼叫地圖的 setXY 方法，傳入位置、字符和顏色
        return map->setXY(x, y, c, color);
    }
    return false;  // 沒有地圖物件時返回失敗
}