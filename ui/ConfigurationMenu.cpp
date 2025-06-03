#include "ConfigurationMenu.h"
#include "../core/GameConfig.h"
#include "../core/GameColors.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
using namespace std;

// 建構函數：初始化配置選單，設定遊戲配置和顏色系統
ConfigurationMenu::ConfigurationMenu(GameConfig* gameConfig, GameColors* gameColors)
    : config(gameConfig), colors(gameColors) {}

// 獲取有效整數輸入：持續提示直到用戶輸入有效範圍內的整數
int ConfigurationMenu::getValidIntInput(int minVal, int maxVal, const string& prompt) {
    int value;
    while(true) {
        cout << colors->CYAN << prompt << " (" << minVal << "-" << maxVal << "): " << colors->RESET;
        if(cin >> value && value >= minVal && value <= maxVal) {
            return value;  // 輸入有效，返回數值
        } else {
            // 輸入無效，顯示錯誤訊息並重試
            cout << colors->RED << "Invalid input! Please enter a number between "
                 << minVal << " and " << maxVal << "." << colors->RESET << endl;
            cin.clear();                // 清除錯誤狀態
            cin.ignore(1000, '\n');     // 忽略錯誤輸入
        }
    }
}

// 獲取有效浮點數輸入：持續提示直到用戶輸入有效範圍內的浮點數
double ConfigurationMenu::getValidDoubleInput(double minVal, double maxVal, const string& prompt) {
    double value;
    while(true) {
        cout << colors->CYAN << prompt << " (" << minVal << "-" << maxVal << "): " << colors->RESET;
        if(cin >> value && value >= minVal && value <= maxVal) {
            return value;  // 輸入有效，返回數值
        } else {
            // 輸入無效，顯示錯誤訊息並重試
            cout << colors->RED << "Invalid input! Please enter a number between "
                 << minVal << " and " << maxVal << "." << colors->RESET << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// 顯示主配置選單：提供快速設定、自訂設定等選項
void ConfigurationMenu::showMainConfigMenu() {
    while(true) {
        system("cls");  // 清除螢幕
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
            case 1:  // 快速設定（預設配置）
                showPresetMenu();
                break;
            case 2:  // 自訂設定
                showCustomConfigMenu();
                break;
            case 3:  // 查看當前設定
                config->displayCurrentConfig(colors);
                cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
                getch();
                break;
            case 4:  // 查看參數指南
                config->displayParameterGuide(colors);
                cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
                getch();
                break;
            case 5:  // 切換顏色模式
                config->setColorsEnabled(!config->getColorsEnabled());
                cout << colors->GREEN << "\nColor mode " << (config->getColorsEnabled() ? "enabled" : "disabled") << "!" << colors->RESET << endl;
                cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
                getch();
                break;
            case 6:  // 重置為預設設定
                *config = GameConfig();
                cout << colors->GREEN << "\nSettings reset to default values!" << colors->RESET << endl;
                cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
                getch();
                break;
            case 7:  // 開始遊戲
                return;
        }
    }
}

// 顯示預設配置選單：提供四種難度預設（休閒、經典、挑戰、極限）
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
        case 1:  // 休閒模式
            config->applyPreset("CASUAL");
            cout << colors->GREEN << "\nCASUAL preset applied!" << colors->RESET << endl;
            break;
        case 2:  // 經典模式
            config->applyPreset("CLASSIC");
            cout << colors->BLUE << "\nCLASSIC preset applied!" << colors->RESET << endl;
            break;
        case 3:  // 挑戰模式
            config->applyPreset("CHALLENGE");
            cout << colors->YELLOW << "\nCHALLENGE preset applied!" << colors->RESET << endl;
            break;
        case 4:  // 極限模式
            config->applyPreset("EXTREME");
            cout << colors->RED << "\nEXTREME preset applied!" << colors->RESET << endl;
            break;
        case 5:  // 返回主選單
            return;
    }

    if(choice >= 1 && choice <= 4) {
        cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
        getch();
    }
}

// 顯示自訂配置選單：提供細部調整各項遊戲參數的選項
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
            case 1: adjustMapDimensions(); break;      // 調整地圖尺寸
            case 2: adjustGameSpeed(); break;          // 調整遊戲速度
            case 3: adjustVehicleSettings(); break;    // 調整車輛設定
            case 4: adjustObstacleCount(); break;      // 調整障礙物數量
            case 5: adjustPlayerLives(); break;        // 調整玩家生命
            case 6: adjustScoringSettings(); break;    // 調整計分設定
            case 7:  // 切換道具系統
                config->setPowerUpsEnabled(!config->getPowerUpsEnabled());
                cout << colors->GREEN << "\nPower-ups " << (config->getPowerUpsEnabled() ? "enabled" : "disabled") << "!" << colors->RESET << endl;
                cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
                getch();
                break;
            case 8:  // 返回主選單
                return;
        }
    }
}

// 調整地圖尺寸：設定地圖的寬度和高度
void ConfigurationMenu::adjustMapDimensions() {
    cout << colors->CYAN << "\n--- Map Dimensions Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current size: " << colors->WHITE << config->getMapWidth() << " x " << config->getMapHeight() << colors->RESET << endl;

    int width = getValidIntInput(20, 80, "Enter map width");
    int height = getValidIntInput(8, 20, "Enter map height");

    // 嘗試更新地圖尺寸並顯示結果
    if(config->setMapWidth(width) && config->setMapHeight(height)) {
        cout << colors->GREEN << "Map dimensions updated successfully!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Error updating map dimensions." << colors->RESET << endl;
    }
    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

// 調整遊戲速度：設定遊戲更新的時間間隔（毫秒）
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

// 調整車輛設定：設定車輛速度、數量和間距
void ConfigurationMenu::adjustVehicleSettings() {
    cout << colors->CYAN << "\n--- Vehicle Settings Configuration ---" << colors->RESET << endl;
    cout << colors->YELLOW << "Current vehicle speed: " << colors->WHITE << config->getVehicleSpeed() << "x" << colors->RESET << endl;
    cout << colors->YELLOW << "Current vehicles per lane: " << colors->WHITE << config->getVehicleCount() << colors->RESET << endl;
    cout << colors->YELLOW << "Current vehicle gap: " << colors->WHITE << config->getVehicleGap() << " units" << colors->RESET << endl;

    int speed = getValidIntInput(1, 5, "Enter vehicle speed multiplier");
    int count = getValidIntInput(1, 8, "Enter vehicles per lane");
    int gap = getValidIntInput(3, 15, "Enter vehicle gap");

    // 同時更新所有車輛設定
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

// 調整障礙物數量：設定地圖上的障礙物總數
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

// 調整玩家生命：設定玩家的初始生命數
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

// 調整計分設定：設定分數倍率
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