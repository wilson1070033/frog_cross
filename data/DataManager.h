#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "DataStructures.h"
#include <vector>
#include <string>
using namespace std;

class GameColors;

class DataManager {
private:
    const string DATA_FOLDER = "gamedata";
    const string USERS_FILE = "gamedata/users.dat";
    const string LEADERBOARD_FILE = "gamedata/leaderboard.dat";
    const string CURRENT_USER_FILE = "gamedata/current_user.dat";

    UserProfile currentUser;
    vector<UserProfile> allUsers;
    vector<LeaderboardEntry> leaderboard;
    vector<GameRecord> userGameHistory;
    GameColors* colors;

    void ensureDataDirectory();
    void loadUsers();
    void saveUsers();
    void loadLeaderboard();
    void saveLeaderboard();
    void loadUserGameHistory();
    void saveUserGameHistory();

public:
    DataManager(GameColors* gameColors);

    void initialize();
    bool registerUser(const string& username, const string& email);
    bool loginUser(const string& username);
    void saveCurrentUser();
    void loadCurrentUser();

    void recordGameResult(int score, int livesUsed, double duration,
                         const string& difficulty, int powerUpsCollected);

    void displayUserProfile() const;
    void displayLeaderboard() const;
    void displayGameHistory() const;

    const UserProfile& getCurrentUser() const;
    bool hasLoggedInUser() const;
    void logoutUser();
    string getUserStatsSummary() const;
};

#endif // DATAMANAGER_H