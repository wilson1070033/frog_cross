#include "GameUtils.h"
#include <cstdlib>
#include <algorithm>
using namespace std;

// 生成指定範圍內的隨機整數
// 參數：n1 = 最小值，n2 = 最大值
// 返回：n1 到 n2 之間的隨機整數（包含邊界值）
int GameUtils::randBtw(int n1, int n2) {
    // 公式解析：
    // rand() % (n2 - n1 + 1) 生成 0 到 (n2-n1) 的隨機數
    // 再加上 n1，得到 n1 到 n2 的隨機數
    // +1 是為了包含 n2 這個邊界值
    return n1 + rand() % (n2 - n1 + 1);
}

// 在車道中生成車輛位置，確保車輛之間有適當間距
// 參數：laneWidth = 車道寬度，vehicleCount = 車輛數量，minGap = 最小間距
// 返回：包含所有車輛 X 座標的向量
vector<int> GameUtils::generateVehiclePositions(int laneWidth, int vehicleCount, int minGap) {
    vector<int> positions;  // 儲存車輛位置的容器

    // 邊界檢查：如果不需要生成車輛，直接返回空向量
    if(vehicleCount == 0) return positions;

    // === 第一階段：計算空間需求和可行性 ===

    // 計算所有車輛之間需要的總間距空間
    // 例如：3輛車需要2個間距 = (3-1) * minGap
    int totalGapSpace = (vehicleCount - 1) * minGap;

    // 計算扣除間距後的可用空間
    // 這個空間用來放置車輛本身和額外的隨機間距
    int availableSpace = laneWidth - totalGapSpace;

    // === 第二階段：調整車輛數量（如果空間不足） ===

    // 檢查可用空間是否足夠放置所有車輛
    if(availableSpace < vehicleCount) {
        // 重新計算可容納的最大車輛數量
        // 每輛車至少需要 1 個位置 + minGap 間距
        vehicleCount = max(1, availableSpace / (minGap + 1));

        // 重新計算總間距空間和可用空間
        totalGapSpace = (vehicleCount - 1) * minGap;
        availableSpace = laneWidth - totalGapSpace;
    }

    // === 第三階段：生成第一輛車的位置 ===

    // 在可用空間內隨機選擇第一輛車的起始位置
    // max(0, ...) 確保不會產生負數位置
    int firstPosition = randBtw(0, max(0, availableSpace - vehicleCount));
    positions.push_back(firstPosition);

    // === 第四階段：依序生成其他車輛位置 ===

    for(int i = 1; i < vehicleCount; i++) {
        int lastPosition = positions[i-1];  // 獲取前一輛車的位置

        // 計算下一輛車的位置：
        // lastPosition + 1：確保至少間隔 1 個位置（車輛本身的寬度）
        // randBtw(minGap, minGap + 2)：在最小間距基礎上增加 0-2 的隨機變化
        int nextPosition = lastPosition + 1 + randBtw(minGap, minGap + 2);

        // 邊界檢查：確保車輛不會超出車道範圍
        if(nextPosition >= laneWidth) {
            nextPosition = laneWidth - 1;  // 限制在車道邊界內
        }

        positions.push_back(nextPosition);  // 將位置加入結果向量
    }

    return positions;  // 返回所有車輛位置
}