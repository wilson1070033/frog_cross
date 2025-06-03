#include "GameStats.h"
#include "../core/GameColors.h"
#include <iostream>
#include <algorithm>
using namespace std;

// 建構函數：初始化所有統計數據為零，記錄遊戲開始時間
GameStats::GameStats(GameColors* gameColors)
    : totalMoves(0), successfulCrossings(0), collisions(0),
      survivalTime(0.0), maxCombo(0), currentCombo(0),
      powerUpsCollected(0), perfectCrossings(0), bestTime(0.0), colors(gameColors) {
    gameStartTime = time(nullptr);  // 記錄遊戲開始的時間戳
}

// 記錄移動：每次玩家移動時調用
void GameStats::recordMove() {
    totalMoves++;
}

// 記錄成功過關：更新過關數、連擊數，檢查是否為完美過關
void GameStats::recordCrossing() {
    successfulCrossings++;
    currentCombo++;                              // 增加當前連擊數
    maxCombo = max(maxCombo, currentCombo);      // 更新最大連擊紀錄
    if(collisions == 0) perfectCrossings++;     // 無碰撞過關為完美過關
}

// 記錄碰撞：增加碰撞數，重置當前連擊
void GameStats::recordCollision() {
    collisions++;
    currentCombo = 0;  // 碰撞會中斷連擊
}

// 更新生存時間：累積遊戲時間，更新最佳時間紀錄
void GameStats::updateSurvivalTime(double time) {
    survivalTime += time;
    bestTime = max(bestTime, survivalTime);  // 追蹤最長生存時間
}

// 記錄道具收集：增加道具數量，保存道具類型
void GameStats::recordPowerUp(PowerUp::Type type) {
    powerUpsCollected++;
    collectedPowerUps.push_back(type);  // 保存收集的道具類型歷史
}

// 計算效率分數：成功過關次數除以總移動次數的百分比
double GameStats::getEfficiencyScore() const {
    if(totalMoves == 0) return 0.0;
    return (double)successfulCrossings / totalMoves * 100.0;
}

// 顯示統計資料：格式化輸出所有遊戲統計數據
void GameStats::displayStats() const {
    cout << colors->BRIGHT_CYAN << "=== ENHANCED GAME STATISTICS ===" << colors->RESET << endl;
    cout << colors->YELLOW << "Total Moves: " << colors->WHITE << totalMoves << endl;
    cout << colors->YELLOW << "Successful Crossings: " << colors->WHITE << successfulCrossings << endl;
    cout << colors->YELLOW << "Perfect Crossings: " << colors->WHITE << perfectCrossings << endl;
    cout << colors->YELLOW << "Collisions: " << colors->WHITE << collisions << endl;
    cout << colors->YELLOW << "Survival Time: " << colors->WHITE << (int)survivalTime / 1000 << "s" << endl;
    cout << colors->YELLOW << "Best Time: " << colors->WHITE << (int)bestTime / 1000 << "s" << endl;
    cout << colors->YELLOW << "Max Combo: " << colors->WHITE << maxCombo << endl;
    cout << colors->YELLOW << "Power-ups Collected: " << colors->WHITE << powerUpsCollected << endl;
    cout << colors->YELLOW << "Efficiency: " << colors->WHITE << (int)getEfficiencyScore() << "%" << endl;
    cout << colors->BRIGHT_CYAN << "=================================" << colors->RESET << endl;
}

// Getter 函數群：返回各種統計數據
int GameStats::getCurrentCombo() const { return currentCombo; }           // 獲取當前連擊數
int GameStats::getPowerUpsCollected() const { return powerUpsCollected; } // 獲取道具收集數
double GameStats::getSurvivalTimeSeconds() const { return survivalTime / 1000.0; } // 獲取生存時間（秒）
int GameStats::getCollisions() const { return collisions; }              // 獲取碰撞次數