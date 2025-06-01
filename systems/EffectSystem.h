#ifndef EFFECTSYSTEM_H
#define EFFECTSYSTEM_H

#include <vector>
#include <string>
using namespace std;

class GameColors;

class EffectSystem {
private:
    struct Effect {
        int x, y;
        string text;
        string color;
        int duration;
        Effect(int x, int y, string text, string col, int dur);
    };

    vector<Effect> activeEffects;
    GameColors* colors;

public:
    EffectSystem(GameColors* gameColors);

    void addEffect(int x, int y, string text, string color = "", int duration = 3);
    void update();
    void render(int mapWidth) const;
    void clear();
    bool hasActiveEffects() const;
};

#endif // EFFECTSYSTEM_H