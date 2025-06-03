#include "GameTimer.h"
using namespace std;
using namespace std::chrono;

// 建構函數：初始化計時器，設定初始時間點
GameTimer::GameTimer() : deltaTime(0.0) {
    // 記錄當前高精度時間作為起始時間點
    // high_resolution_clock 提供系統可用的最高精度時鐘
    lastTime = high_resolution_clock::now();
}

// 更新函數：計算自上次更新以來經過的時間（毫秒），並返回時間差
double GameTimer::update() {
    // 獲取當前的高精度時間
    auto currentTime = high_resolution_clock::now();

    // 計算時間差：
    // 1. (currentTime - lastTime) 得到時間間隔
    // 2. duration<double, milli> 將時間間隔轉換為毫秒（double類型）
    // 3. .count() 獲取實際的數值
    deltaTime = duration<double, milli>(currentTime - lastTime).count();

    // 更新 lastTime 為當前時間，準備下次計算
    lastTime = currentTime;

    // 返回計算出的時間差（毫秒）
    return deltaTime;
}

// 取得函數：返回最近一次計算的時間差值
double GameTimer::getDeltaTime() const {
    return deltaTime;  // 返回儲存的時間差值（毫秒）
}