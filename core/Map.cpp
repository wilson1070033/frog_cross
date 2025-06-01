#include "Map.h"
#include "GameColors.h"
#include "GameUtils.h"
#include "GameConfig.h"
#include "../systems/GameStats.h"
#include "../systems/EffectSystem.h"
#include "../systems/PowerUp.h"
#include "../data/DataManager.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ColoredChar implementation
ColoredChar::ColoredChar(char c, string col) : character(c), color(col) {}

string ColoredChar::toString() const {
    return color + character + "\033[0m";
}

// Map implementation
Map::Map(GameConfig* gameConfig, GameColors* gameColors, GameUtils* gameUtils)
    : config(gameConfig), colors(gameColors), utils(gameUtils), stats(nullptr), effects(nullptr) {
    W = config->getMapWidth();
    H = config->getMapHeight();
    lives = config->getPlayerLives();
    R = new int[H];
    generateRoadPattern();
    initializeMap();
}

Map::~Map() {
    delete[] R;
    for(auto powerUp : powerUps) {
        delete powerUp;
    }
}

void Map::generateRoadPattern() {
    R[0] = 1;
    R[H-1] = 1;

    for(int i = 1; i < H-1; i++) {
        if(i == H/2) {
            R[i] = 1;
        } else {
            R[i] = 0;
        }
    }

    int roadCount = 0;
    for(int i = 0; i < H; i++) {
        if(R[i] == 0) roadCount++;
    }

    if(roadCount < 2) {
        for(int i = 1; i < H-1; i += 2) {
            R[i] = 0;
        }
    }
}

void Map::initializeMap() {
    grid = vector<vector<ColoredChar>>(H, vector<ColoredChar>(W, ColoredChar('.', colors->WHITE)));

    for(int i = 0; i < H; i++) {
        if(R[i] == 1) {
            for(int j = 0; j < W; j++) {
                grid[i][j] = ColoredChar('=', colors->BRIGHT_YELLOW);
            }
        }
    }

    g0 = grid;

    if(config && config->getPowerUpsEnabled()) {
        generatePowerUps();
    }

    for(int i = 0; i < H; i++) {
        if(R[i] == 0) {
            if(i % 2 == 1) {
                grid[i][0] = ColoredChar('C', colors->BRIGHT_CYAN);
            } else {
                grid[i][W - 1] = ColoredChar('C', colors->BRIGHT_CYAN);
            }
        }
    }

    for(int i = 0; i < W; i += max(6, W/8)) {
        if(i < W) {
            grid[0][i] = ColoredChar('*', colors->BRIGHT_YELLOW);
            if(H > 1) grid[H-1][i] = ColoredChar('~', colors->BRIGHT_BLUE);
        }
    }
}

void Map::generatePowerUps() {
    int powerUpCount = max(1, W / 15);

    for(int i = 0; i < powerUpCount; i++) {
        PowerUp::Type type = static_cast<PowerUp::Type>(rand() % 4);
        int x = utils->randBtw(1, W - 2);
        int y = utils->randBtw(1, H - 2);

        if(R[y] == 1) {
            powerUps.push_back(new PowerUp(type, x, y, colors));
        }
    }
}

void Map::updatePowerUps() {
    powerUps.erase(
        remove_if(powerUps.begin(), powerUps.end(),
            [](PowerUp* p) {
                if(!p->update()) {
                    delete p;
                    return true;
                }
                return false;
            }),
        powerUps.end()
    );

    for(auto powerUp : powerUps) {
        if(powerUp->isActive()) {
            setXY(powerUp->getX(), powerUp->getY(), powerUp->getSymbol(), powerUp->getColor());
        }
    }
}

void Map::fastDraw(DataManager* dataManager) {
    #ifdef _WIN32
        system("cls");
    #else
        cout << "\033[2J\033[H";
    #endif

    cout << colors->BRIGHT_CYAN << "===== ENHANCED FROG CROSSING GAME =====" << colors->RESET << endl;

    if(dataManager->hasLoggedInUser()) {
        cout << colors->BRIGHT_GREEN << dataManager->getUserStatsSummary() << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Score: " << colors->WHITE << score << colors->YELLOW << " | Lives: " << colors->RESET;

    for(int i = 0; i < lives; i++) {
        cout << colors->BRIGHT_GREEN << "[*]" << colors->RESET;
    }
    for(int i = lives; i < 3; i++) {
        cout << colors->RED << "[ ]" << colors->RESET;
    }

    if(stats) {
        cout << colors->YELLOW << " | Combo: " << colors->WHITE << stats->getCurrentCombo();
        cout << colors->YELLOW << " | Efficiency: " << colors->WHITE << (int)stats->getEfficiencyScore() << "%";
        cout << colors->YELLOW << " | Power-ups: " << colors->WHITE << stats->getPowerUpsCollected();
    }
    cout << endl;

    cout << colors->CYAN << "Controls: Arrow Keys | Goal: Reach the " << colors->BRIGHT_YELLOW << "stars" << colors->CYAN << "!" << colors->RESET << endl;

    for(int i = 0; i < W; i++) cout << colors->BRIGHT_BLUE << "=" << colors->RESET;
    cout << endl;

    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            if(config && config->getColorsEnabled()) {
                cout << grid[i][j].toString();
            } else {
                cout << grid[i][j].character;
            }
        }
        cout << endl;
    }

    for(int i = 0; i < W; i++) cout << colors->BRIGHT_BLUE << "=" << colors->RESET;
    cout << endl;

    if(effects && effects->hasActiveEffects()) {
        cout << colors->BRIGHT_MAGENTA << "*** SPECIAL EFFECTS ACTIVE ***" << colors->RESET << endl;
    }

    if(config && config->getPowerUpsEnabled() && !powerUps.empty()) {
        cout << colors->BRIGHT_CYAN << "Active Power-ups: " << powerUps.size() << colors->RESET << endl;
    }

    grid = g0;
    updatePowerUps();
}

bool Map::setXY(int x, int y, char c, string color) {
    if(y >= 0 && y < H && x >= 0 && x < W) {
        if(color.empty()) {
            switch(c) {
                case 'F':
                    color = colors->BRIGHT_GREEN;
                    break;
                case 'C':
                    color = colors->BRIGHT_CYAN;
                    break;
                case 'M':
                    color = colors->BRIGHT_RED;
                    break;
                case 'O':
                    color = colors->YELLOW;
                    break;
                default:
                    color = colors->WHITE;
                    break;
            }
        }

        if(c == 'O') g0[y][x] = ColoredChar(c, color);
        if(c == 'F' && grid[y][x].character == 'C') return false;
        if(c == 'F' && grid[y][x].character == 'M') return false;
        if(c == 'C' && grid[y][x].character == 'F') return false;
        if(c == 'M' && grid[y][x].character == 'F') return false;

        grid[y][x] = ColoredChar(c, color);
    }
    return true;
}

PowerUp* Map::checkPowerUpCollision(int x, int y) {
    for(auto powerUp : powerUps) {
        if(powerUp->isActive() && powerUp->getX() == x && powerUp->getY() == y) {
            return powerUp;
        }
    }
    return nullptr;
}

void Map::collectPowerUp(PowerUp* powerUp) {
    if(stats) {
        stats->recordPowerUp(powerUp->getType());
    }

    if(effects) {
        effects->addEffect(powerUp->getX(), powerUp->getY(),
                         powerUp->getDescription(),
                         powerUp->getColor(), 7);
    }

    switch(powerUp->getType()) {
        case PowerUp::EXTRA_LIFE:
            lives++;
            break;
        case PowerUp::DOUBLE_SCORE:
            score += 50;
            break;
    }

    powerUp->update();
}

void Map::addScore(int i, GameConfig* config) {
    double multiplier = config ? config->getScoreMultiplier() : 1.0;
    int actualScore = (int)(i * multiplier);
    score += actualScore;

    if(stats) {
        stats->recordMove();
        if(effects && actualScore > 1) {
            effects->addEffect(W/2, H/2, "+" + to_string(actualScore), colors->BRIGHT_GREEN, 3);
        }
    }
}

void Map::loseLife() {
    if(lives > 0) {
        lives--;
        if(stats) stats->recordCollision();
        if(effects) effects->addEffect(W/2, H/2, "COLLISION!", colors->BRIGHT_RED, 5);
    }
}

int Map::getLives() const { return lives; }
int Map::getScore() const { return score; }
int Map::getWidth() const { return W; }
int Map::getHeight() const { return H; }

bool Map::getO(int x, int y) {
    if(y >= 0 && y < H && x >= 0 && x < W) {
        if(g0[y][x].character == 'O') return true;
    }
    return false;
}

void Map::resetFrog(int x, int y) {
    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++) {
            if(grid[i][j].character == 'F') {
                grid[i][j] = g0[i][j];
            }
        }
    }
}

char Map::getCharAt(int x, int y) {
    if(y >= 0 && y < H && x >= 0 && x < W) {
        return grid[y][x].character;
    }
    return '.';
}

void Map::setStats(GameStats* s) { stats = s; }
void Map::setEffects(EffectSystem* e) { effects = e; }

bool Map::isScoreZone(int x, int y) {
    return (y <= 2 && g0[y][x].character == '.');
}

bool Map::isRoad(int y) {
    if(y >= 0 && y < H) {
        return R[y] == 0;
    }
    return false;
}