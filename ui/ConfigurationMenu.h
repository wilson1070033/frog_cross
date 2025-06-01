#ifndef CONFIGURATIONMENU_H
#define CONFIGURATIONMENU_H

#include <string>
using namespace std;

class GameConfig;
class GameColors;

class ConfigurationMenu {
private:
    GameConfig* config;
    GameColors* colors;

    int getValidIntInput(int minVal, int maxVal, const string& prompt);
    double getValidDoubleInput(double minVal, double maxVal, const string& prompt);
    void adjustMapDimensions();
    void adjustGameSpeed();
    void adjustVehicleSettings();
    void adjustObstacleCount();
    void adjustPlayerLives();
    void adjustScoringSettings();

public:
    ConfigurationMenu(GameConfig* gameConfig, GameColors* gameColors);
    void showMainConfigMenu();
    void showPresetMenu();
    void showCustomConfigMenu();
};

#endif // CONFIGURATIONMENU_H