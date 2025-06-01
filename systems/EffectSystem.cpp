#include "EffectSystem.h"
#include "../core/GameColors.h"
#include <iostream>
#include <algorithm>
using namespace std;

EffectSystem::Effect::Effect(int x, int y, string text, string col, int dur)
    : x(x), y(y), text(text), color(col), duration(dur) {}

EffectSystem::EffectSystem(GameColors* gameColors) : colors(gameColors) {}

void EffectSystem::addEffect(int x, int y, string text, string color, int duration) {
    if(color.empty()) color = colors->WHITE;
    activeEffects.push_back(Effect(x, y, text, color, duration));
}

void EffectSystem::update() {
    activeEffects.erase(
        remove_if(activeEffects.begin(), activeEffects.end(),
            [](Effect& e) { return --e.duration <= 0; }),
        activeEffects.end()
    );
}

void EffectSystem::render(int mapWidth) const {
    for(const auto& effect : activeEffects) {
        cout << "\033[" << (effect.y + 3) << ";" << (effect.x + 1) << "H"
             << effect.color << effect.text << colors->RESET;
    }
}

void EffectSystem::clear() {
    activeEffects.clear();
}

bool EffectSystem::hasActiveEffects() const {
    return !activeEffects.empty();
}