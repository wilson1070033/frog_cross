#include "EffectSystem.h"
#include "../core/GameColors.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Effect 內部結構建構函數：初始化單個特效的屬性
EffectSystem::Effect::Effect(int x, int y, string text, string col, int dur)
    : x(x), y(y), text(text), color(col), duration(dur) {}

// 特效系統建構函數：初始化顏色系統
EffectSystem::EffectSystem(GameColors* gameColors) : colors(gameColors) {}

// 添加新特效：在指定位置創建帶顏色和持續時間的文字特效
void EffectSystem::addEffect(int x, int y, string text, string color, int duration) {
    if(color.empty()) color = colors->WHITE;  // 沒指定顏色時使用白色
    activeEffects.push_back(Effect(x, y, text, color, duration));
}

// 更新特效：減少持續時間，移除過期的特效
void EffectSystem::update() {
    activeEffects.erase(
        remove_if(activeEffects.begin(), activeEffects.end(),
            [](Effect& e) { return --e.duration <= 0; }),  // 時間歸零的特效移除
        activeEffects.end()
    );
}

// 渲染特效：使用 ANSI 轉義序列在指定位置顯示彩色文字
void EffectSystem::render(int mapWidth) const {
    for(const auto& effect : activeEffects) {
        // \033[y;xH 移動游標到指定位置
        cout << "\033[" << (effect.y + 3) << ";" << (effect.x + 1) << "H"
             << effect.color << effect.text << colors->RESET;
    }
}

// 清除所有特效：移除所有活躍的特效
void EffectSystem::clear() {
    activeEffects.clear();
}

// 檢查是否有活躍特效：返回是否還有特效在顯示
bool EffectSystem::hasActiveEffects() const {
    return !activeEffects.empty();
}