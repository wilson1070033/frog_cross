#include "ConfigurationMenu.h"
#include "../core/GameConfig.h"
#include "../core/GameColors.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
using namespace std;

ConfigurationMenu::ConfigurationMenu(GameConfig* gameConfig, GameColors* gameColors)
    : config(gameConfig), colors(gameColors) {}

int ConfigurationMenu::getValidIntInput(int minVal, int maxVal, const string& prompt) {
    int value;
    while(true) {
        cout << colors->CYAN << prompt << " (" << minVal << "-" << maxVal << "): " << colors->RESET;
        if(cin >> value && value >= minVal && value <= maxVal) {
            return value;
        } else {
            cout << colors->RED << "Invalid input! Please enter a number between "
                 << minVal << " and " << maxVal << "." << colors->RESET << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

double ConfigurationMenu::getValidDoubleInput(double minVal, double maxVal, const string& prompt) {
    double value;
    while(true) {
        cout << colors->CYAN << prompt << " (" << minVal << "-" << maxVal << "): " << colors->RESET;
        if(cin >> value && value >= minVal && value <= maxVal) {
            return value;
        } else {
            cout << colors->RED << "Invalid input! Please enter a number between "
                 << minVal << " and " << maxVal << "." << colors->RESET << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

void ConfigurationMenu::showMainConfigMenu() {
    while(true) {
        system("cls");
        cout << colors->BRIGHT_CYAN << "\n================ ENHANCED GAME CONFIGURATION MENU ================" << colors->RESET << endl;
        cout << colors->YELLOW << "Choose an option to customize your enhanced gaming experience:" << colors->RESET << endl;
        cout << colors->WHITE << "1. Quick Setup (Use preset configurations)" << endl;
        cout << colors->WHITE << "2. Custom Setup (Adjust individual parameters)" << endl;
        cout << colors->WHITE << "3. View Current Settings" << endl;
        cout << colors->WHITE << "4. View Parameter Guide" << endl;
        cout << colors->WHITE << "5. Toggle Color Mode" << endl;
        cout << colors->WHITE << "6. Reset to Default Settings" << endl;
        cout << colors->WHITE << "7. Start Game with Current Settings" << endl;
        cout << colors->BRIGHT_CYAN << "====================================================================" << colors->RESET << endl;

        int choice = getValidIntInput(1, 7, "Enter your choice");

        switch(choice) {
            case 1:
                showPresetMenu();
                break;
            case 2:
                showCustomConfigMenu();
                break;
            case 3:
                config->displayCurrentConfig(colors);
                cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
                getch();
                break;
            case 4:
                config->displayParameterGuide(colors);
                cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
                getch();
                break;
            case 5:
                config->setColorsEnabled(!config->getColorsEnabled());
                cout << colors->GREEN << "\nColor mode " << (config->getColorsEnabled() ? "enabled" : "disabled") << "!" << colors->RESET << endl;
                cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
                getch();
                break;
            case 6:
                *config = GameConfig();
                cout << colors->GREEN << "\nSettings reset to default values!" << colors->RESET << endl;
                cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
                getch();
                break;
            case 7:
                return;
        }
    }
}

void ConfigurationMenu::showPresetMenu() {
    system("cls");
    cout << colors->BRIGHT_CYAN << "\n============== PRESET CONFIGURATIONS ==============" << colors->RESET << endl;
    cout << colors->YELLOW << "Choose a preset that matches your preferred play style:" << colors->RESET << endl;
    cout << colors->GREEN << "1. CASUAL - Relaxed gaming for stress relief" << colors->RESET << endl;
    cout << colors->BLUE << "2. CLASSIC - Traditional frog crossing experience" << colors->RESET << endl;
    cout << colors->YELLOW << "3. CHALLENGE - Test your reflexes and strategy" << colors->RESET << endl;
    cout << colors->RED << "4. EXTREME - Only for the most dedicated players" << colors->RESET << endl;
    cout << colors->WHITE << "5. Return to main menu" << colors->RESET << endl;
    cout << colors->BRIGHT_CYAN << "====================================================" << colors->RESET << endl;

    int choice = getValidIntInput(1, 5, "Select preset");

    switch(choice) {
        case 1:
            config->applyPreset("CASUAL");
            cout << colors->GREEN << "\nCASUAL preset applied!" << colors->RESET << endl;
            break;
        case 2:
            config->applyPreset("CLASSIC");
            cout << colors->BLUE << "\nCLASSIC preset applied!" << colors->RESET << endl;
            break;
        case 3:
            config->applyPreset("CHALLENGE");
            cout << colors->YELLOW << "\nCHALLENGE preset applied!" << colors->RESET << endl;
            break;
        case 4:
            config->applyPreset("EXTREME");
            cout << colors->RED << "\nEXTREME preset applied!" << colors->RESET << endl;
            break;
        case 5:
            return;
    }

    if(choice >= 1 && choice <= 4) {
        cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
        getch();
    }
}

void ConfigurationMenu::showCustomConfigMenu() {
    while(true) {
        system("cls");
        cout << colors->BRIGHT_CYAN << "\n============= CUSTOM CONFIGURATION =============" << colors->RESET << endl;
        cout << colors->YELLOW << "Fine-tune individual game parameters:" << colors->RESET << endl;
        cout << colors->WHITE << "1. Map Dimensions (Width x Height)" << endl;
        cout << colors->WHITE << "2. Game Speed (Update frequency)" << endl;
        cout << colors->WHITE << "3. Vehicle Settings (Speed and count)" << endl;
        cout << colors->WHITE << "4. Obstacle Count" << endl;
        cout << colors->WHITE << "5. Player Lives" << endl;
        cout << colors->WHITE << "6. Scoring Settings" << endl;
        cout << colors->WHITE << "7. Toggle Power-ups" << endl;
        cout << colors->WHITE << "8. Return to main menu" << endl;
        cout << colors->BRIGHT_CYAN << "=================================================" << colors->RESET << endl;

        int choice = getValidIntInput(1, 8, "Select category to adjust");

        switch(choice) {
            case 1:
                adjustMapDimensions();
                break;
            case 2:
                adjustGameSpeed();
                break;
            case 3:
                adjustVehicleSettings();
                break;
            case 4:
                adjustObstacleCount();
                break;
            case 5:
                adjustPlayerLives();
                break;
            case 6:
                adjustScoringSettings();
                break;
            case 7:
                config->setPowerUpsEnabled(!config->getPowerUpsEnabled());
                cout << colors->GREEN << "\nPower-ups " << (config->getPowerUpsEnabled() ? "enabled" : "disabled") << "!" << colors->RESET << endl;
                cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
                getch();
                break;
            case 8:
                return;
        }
    }
}

void ConfigurationMenu::adjustMapDimensions() {
    cout << colors->CYAN << "\n--- Map Dimensions Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current size: " << colors->WHITE << config->getMapWidth() << " x " << config->getMapHeight() << colors->RESET << endl;

    int width = getValidIntInput(20, 80, "Enter map width");
    int height = getValidIntInput(8, 20, "Enter map height");

    if(config->setMapWidth(width) && config->setMapHeight(height)) {
        cout << colors->GREEN << "Map dimensions updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating map dimensions." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

void ConfigurationMenu::adjustGameSpeed() {
    cout << colors->CYAN << "\n--- Game Speed Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current speed: " << colors->WHITE << config->getGameSpeed() << " ms" << colors->RESET << endl;

    int speed = getValidIntInput(50, 500, "Enter game speed (milliseconds)");

    if(config->setGameSpeed(speed)) {
        cout << colors->GREEN << "Game speed updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating game speed." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

void ConfigurationMenu::adjustVehicleSettings() {
    cout << colors->CYAN << "\n--- Vehicle Settings Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current vehicle speed: " << colors->WHITE << config->getVehicleSpeed() << "x" << colors->RESET << endl;
    cout << colors->YELLOW << "Current vehicles per lane: " << colors->WHITE << config->getVehicleCount() << colors->RESET << endl;
    cout << colors->YELLOW << "Current vehicle gap: " << colors->WHITE << config->getVehicleGap() << " units" << colors->RESET << endl;

    int speed = getValidIntInput(1, 5, "Enter vehicle speed multiplier");
    int count = getValidIntInput(1, 8, "Enter vehicles per lane");
    int gap = getValidIntInput(3, 15, "Enter vehicle gap");

    bool success = config->setVehicleSpeed(speed) &&
                  config->setVehicleCount(count) &&
                  config->setVehicleGap(gap);

    if(success) {
        cout << colors->GREEN << "Vehicle settings updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating vehicle settings." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

void ConfigurationMenu::adjustObstacleCount() {
    cout << colors->CYAN << "\n--- Obstacle Count Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current obstacles: " << colors->WHITE << config->getObstacleCount() << colors->RESET << endl;

    int count = getValidIntInput(0, 15, "Enter obstacle count");

    if(config->setObstacleCount(count)) {
        cout << colors->GREEN << "Obstacle count updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating obstacle count." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

void ConfigurationMenu::adjustPlayerLives() {
    cout << colors->CYAN << "\n--- Player Lives Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current lives: " << colors->WHITE << config->getPlayerLives() << colors->RESET << endl;

    int lives = getValidIntInput(1, 9, "Enter player lives");

    if(config->setPlayerLives(lives)) {
        cout << colors->GREEN << "Player lives updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating player lives." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

void ConfigurationMenu::adjustScoringSettings() {
    cout << colors->CYAN << "\n--- Scoring Settings Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current score multiplier: " << colors->WHITE << config->getScoreMultiplier() << "x" << colors->RESET << endl;

    double multiplier = getValidDoubleInput(0.5, 5.0, "Enter score multiplier");

    if(config->setScoreMultiplier(multiplier)) {
        cout << colors->GREEN << "Score multiplier updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating score multiplier." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}