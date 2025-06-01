#include "UserManagementMenu.h"
#include "DataManager.h"
#include "../core/GameColors.h"
#include <iostream>
#include <conio.h>
#include <cstdlib>
using namespace std;

UserManagementMenu::UserManagementMenu(DataManager* dm, GameColors* gameColors)
    : dataManager(dm), colors(gameColors), shouldStartGame(false) {}

string UserManagementMenu::getStringInput(const string& prompt) {
    string input;
    cout << colors->CYAN << prompt << ": " << colors->RESET;
    getline(cin, input);
    return input;
}

bool UserManagementMenu::showUserMenu() {
    shouldStartGame = false;

    while(!shouldStartGame) {
        system("cls");
        cout << colors->BRIGHT_CYAN << "\n============ USER MANAGEMENT ============" << colors->RESET << endl;

        if(dataManager->hasLoggedInUser()) {
            cout << colors->GREEN << "Welcome back, " << dataManager->getCurrentUser().username << "!" << colors->RESET << endl;
            cout << colors->WHITE << "1. View Profile" << endl;
            cout << colors->WHITE << "2. View Game History" << endl;
            cout << colors->WHITE << "3. View Leaderboard" << endl;
            cout << colors->WHITE << "4. Logout" << endl;
            cout << colors->WHITE << "5. Start Game" << endl;
        } else {
            cout << colors->YELLOW << "Please log in or register to continue" << colors->RESET << endl;
            cout << colors->WHITE << "1. Login" << endl;
            cout << colors->WHITE << "2. Register New User" << endl;
            cout << colors->WHITE << "3. View Leaderboard (Guest)" << endl;
            cout << colors->WHITE << "4. Play as Guest" << endl;
        }

        cout << colors->WHITE << "0. Exit Game" << endl;
        cout << colors->BRIGHT_CYAN << "=========================================" << colors->RESET << endl;

        int maxChoice = dataManager->hasLoggedInUser() ? 5 : 4;
        int choice;
        cout << colors->CYAN << "Enter your choice (0-" << maxChoice << "): " << colors->RESET;

        if(!(cin >> choice)) {
            cout << colors->RED << "Invalid input! Please enter a number." << colors->RESET << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            continue;
        }
        cin.ignore();

        if(choice < 0 || choice > maxChoice) {
            cout << colors->RED << "Invalid choice! Please select between 0 and " << maxChoice << "." << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            continue;
        }

        if(choice == 0) {
            cout << colors->YELLOW << "Thank you for playing!" << colors->RESET << endl;
            exit(0);
        }

        if(dataManager->hasLoggedInUser()) {
            handleLoggedInUser(choice);
        } else {
            handleGuestUser(choice);
        }
    }

    return shouldStartGame;
}

void UserManagementMenu::handleLoggedInUser(int choice) {
    switch(choice) {
        case 1:
            dataManager->displayUserProfile();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 2:
            dataManager->displayGameHistory();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 3:
            dataManager->displayLeaderboard();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 4:
            dataManager->logoutUser();
            cout << colors->GREEN << "Successfully logged out!" << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            break;
        case 5:
            shouldStartGame = true;
            break;
        default:
            cout << colors->RED << "Invalid option!" << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            break;
    }
}

void UserManagementMenu::handleGuestUser(int choice) {
    switch(choice) {
        case 1:
            performLogin();
            break;
        case 2:
            performRegistration();
            break;
        case 3:
            dataManager->displayLeaderboard();
            cout << colors->YELLOW << "\nPress any key to continue..." << colors->RESET;
            getch();
            break;
        case 4:
            shouldStartGame = true;
            break;
        default:
            cout << colors->RED << "Invalid option!" << colors->RESET << endl;
            cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
            getch();
            break;
    }
}

void UserManagementMenu::performLogin() {
    cout << colors->BRIGHT_CYAN << "\n============ USER LOGIN ============" << colors->RESET << endl;
    string username = getStringInput("Enter username");

    if(dataManager->loginUser(username)) {
        cout << colors->GREEN << "Login successful! Welcome back, " << username << "!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "User not found! Please check your username or register." << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}

void UserManagementMenu::performRegistration() {
    cout << colors->BRIGHT_CYAN << "\n========== USER REGISTRATION ==========" << colors->RESET << endl;
    string username = getStringInput("Enter desired username");
    string email = getStringInput("Enter email address");

    if(dataManager->registerUser(username, email)) {
        cout << colors->GREEN << "Registration successful!" << colors->RESET << endl;
        dataManager->loginUser(username);
        cout << colors->GREEN << "You are now logged in as " << username << "!" << colors->RESET << endl;
    } else {
        cout << colors->RED << "Username already exists! Please choose a different username." << colors->RESET << endl;
    }

    cout << colors->YELLOW << "Press any key to continue..." << colors->RESET;
    getch();
}