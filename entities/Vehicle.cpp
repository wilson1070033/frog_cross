#include "Vehicle.h"
#include "../core/Map.h"

// 建構函數：初始化車輛基本屬性
Vehicle::Vehicle(int x, int y, char c, int d, Map* gameMap, GameConfig* config, GameColors* gameColors, string col)
    : GameObject(x, y, c, gameMap, config, gameColors, col), d(d), speed(1.0) {}

// 路徑碰撞檢測：檢查從起點到終點的移動路徑上是否撞到青蛙
bool Vehicle::checkPathCollision(int startX, int endX, int y) {
    // 如果起點和終點相同，沒有移動，無碰撞
    if(startX == endX) return false;

    // 決定移動方向：向右(+1) 或向左(-1)
    int direction = (endX > startX) ? 1 : -1;

    // 逐步檢查移動路徑上的每個位置
    for(int x = startX; x != endX; x += direction) {
        // 確保座標在地圖範圍內
        if(x >= 0 && x < map->getWidth() && y >= 0 && y < map->getHeight()) {
            // 檢查該位置是否有青蛙 ('F')
            if(map && map->getCharAt(x, y) == 'F') {
                return true;  // 發現青蛙，返回碰撞
            }
        }
    }
    return false;  // 路徑安全，無碰撞
}