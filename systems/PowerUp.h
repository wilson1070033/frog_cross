#ifndef POWERUP_H
#define POWERUP_H

#include <string>
using namespace std;

class GameColors;

class PowerUp {
public:
    enum Type {
        SHIELD,
        SPEED_BOOST,
        DOUBLE_SCORE,
        EXTRA_LIFE
    };

private:
    Type type;
    int x, y;
    int duration;
    bool active;
    GameColors* colors;

public:
    PowerUp(Type t, int posX, int posY, GameColors* gameColors);

    bool update();
    Type getType() const;
    int getX() const;
    int getY() const;
    bool isActive() const;
    char getSymbol() const;
    string getColor() const;
    string getDescription() const;
};

#endif // POWERUP_H