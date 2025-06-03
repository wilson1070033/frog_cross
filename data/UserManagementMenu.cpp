#include "UserManagementMenu.h"
#include "DataManager.h"
#include "../core/GameColors.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
using namespace std;

// 建構函數：初始化資料管理器和顏色系統
UserManagementMenu::UserManagementMenu(DataManager* dm, GameColors* gameColors)
    : dataManager(dm), colors(gameColors), shouldStartGame(false) {}

// 獲取用戶字串輸入的輔助函數
string UserManagementMenu::getStringInput(const string& prompt) {
    string input;
    cout << colors->CYAN << prompt << ": " << colors->RESET;
    getline(cin, input);  // 讀取整行輸入（包含空格）
    return input;
}

// 顯示用戶管理主選單：根據登入狀態顯示不同選項
bool UserManagementMenu::showUserMenu() {
    shouldStartGame = false;

    // 持續顯示選單直到用戶選擇開始遊戲
    while(!shouldStartGame) {
        system("cls");  // 清除螢幕
        cout << colors->BRIGHT_CYAN << "\n============ USER MANAGEMENT ============" << colors->RESET << endl;

        // 根據用戶登入狀態顯示不同選單
        if(dataManager->hasLoggedInUser()) {
            // 已登入用戶選單
            cout << colors->GREEN << "Welcome back, " << dataManager->getCurrentUser().username << "!" << colors->RESET << endl;
            cout << colors->WHITE << "1. View Profile" << endl;
            cout << colors->WHITE << "2. View Game History" << endl;
            cout << colors->WHITE << "3. View Leaderboard" << endl;
            cout << colors->WHITE << "4. Logout" << endl;
            cout << colors->WHITE << "5. Start Game" << endl;
        } else {
            // 未登入用戶選單
            cout << colors->YELLOW << "Please log in or register to continue" << colors->RESET << endl;
            cout << colors->WHITE << "1. Login" << endl;
            cout << colors->WHITE << "2. Register New User" << endl;
            cout << colors->WHITE << "3. View Leaderboard (Guest)" << endl;
            cout << colors->WHITE << "4. Play as Guest" << endl;
        }

        cout << colors->WHITE << "0. Exit Game" << endl;
        cout << colors->BRIGHT_CYAN << "=========================================" << colors->RESET << endl;

        // 輸入驗證：檢查選項範圍
        int maxChoice = dataManager->hasLoggedInUser() ? 5 : 4;
        int choice;
        cout << colors->CYAN << "Enter your choice (0-" << maxChoice << "): " << colors->RESET;

        // 檢查輸入是否為有效數字
        if(!(cin >> choice)) {
            cout << colors->RED << "Invalid input! Please enter a number." << colors->RESET << endl;
            cin.clear();                // 清除錯誤狀態
            cin.ignore(1000, '\n');     // 忽略錯誤輸入
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            continue;
        }
        cin.ignore();  // 清除輸入緩衝區

        // 檢查選項是否在有效範圍內
        if(choice < 0 || choice > maxChoice) {
            cout << colors->RED << "Invalid choice! Please select between 0 and " << maxChoice << "." << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            continue;
        }

        // 處理退出遊戲
        if(choice == 0) {
            cout << colors->YELLOW << "Thank you for playing!" << colors->RESET << endl;
            exit(0);
        }

        // 根據登入狀態分派處理
        if(dataManager->hasLoggedInUser()) {
            handleLoggedInUser(choice);
        } else {
            handleGuestUser(choice);
        }
    }

    return shouldStartGame;
}

// 處理已登入用戶的選單選項
void UserManagementMenu::handleLoggedInUser(int choice) {
    switch(choice) {
        case 1:  // 查看個人檔案
            dataManager->displayUserProfile();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 2:  // 查看遊戲歷史
            dataManager->displayGameHistory();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 3:  // 查看排行榜
            dataManager->displayLeaderboard();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 4:  // 登出
            dataManager->logoutUser();
            cout << colors->GREEN << "Successfully logged out!" << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            break;
        case 5:  // 開始遊戲
            shouldStartGame = true;
            break;
        default:
            cout << colors->RED << "Invalid option!" << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            break;
    }
}

// 處理未登入用戶（訪客）的選單選項
void UserManagementMenu::handleGuestUser(int choice) {
    switch(choice) {
        case 1:  // 登入
            performLogin();
            break;
        case 2:  // 註冊
            performRegistration();
            break;
        case 3:  // 以訪客身分查看排行榜
            dataManager->displayLeaderboard();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 4:  // 以訪客身分開始遊戲
            shouldStartGame = true;
            break;
        default:
            cout << colors->RED << "Invalid option!" << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            break;
    }
}

// 執行用戶登入流程
void UserManagementMenu::performLogin() {
    cout << colors->BRIGHT_CYAN << "\n============ USER LOGIN ============" << colors->RESET << endl;
    string username = getStringInput("Enter username");

    // 嘗試登入並顯示結果
    if(dataManager->loginUser(username)) {
        cout << colors->GREEN << "Login successful! Welcome back, " << username << "!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "User not found! Please check your username or register." << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

// 執行用戶註冊流程
void UserManagementMenu::performRegistration() {
    cout << colors->BRIGHT_CYAN << "\n========== USER REGISTRATION ==========" << colors->RESET << endl;
    string username = getStringInput("Enter desired username");
    string email = getStringInput("Enter email address");

    // 嘗試註冊並自動登入
    if(dataManager->registerUser(username, email)) {
        cout << colors->GREEN << "Registration successful!" << colors->RESET << endl;
        dataManager->loginUser(username);  // 註冊成功後自動登入
        cout << colors->GREEN << "You are now logged in as " << username << "!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Username already exists! Please choose a different username." << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}