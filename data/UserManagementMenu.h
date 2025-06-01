#ifndef USERMANAGEMENTMENU_H
#define USERMANAGEMENTMENU_H

#include <string>
using namespace std;

class DataManager;
class GameColors;

class UserManagementMenu {
private:
    DataManager* dataManager;
    GameColors* colors;
    bool shouldStartGame;

    string getStringInput(const string& prompt);
    void handleLoggedInUser(int choice);
    void handleGuestUser(int choice);
    void performLogin();
    void performRegistration();

public:
    UserManagementMenu(DataManager* dm, GameColors* gameColors);
    bool showUserMenu();
};

#endif // USERMANAGEMENTMENU_H