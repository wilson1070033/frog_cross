#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
using namespace std;

// 前向宣告
class Map;
class GameConfig;
class GameColors;

class GameObject {
protected:
    int x, y;
    const char c;
    string color;
    Map* map;
    GameConfig* currentConfig;
    GameColors* colors;
    double moveTimer;

public:
    GameObject(int x, int y, char c, Map* gameMap, GameConfig* config, GameColors* gameColors, string col = "");
    virtual ~GameObject() = default;

    char getIcon();
    string getColor();
    int getX();
    int getY();
    virtual bool move(double deltaTime) = 0;
    bool setXY(int x, int y, char c);
};

#endif // GAMEOBJECT_H