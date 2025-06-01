#include "DataManager.h"
#include "../core/GameColors.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <filesystem>
using namespace std;

DataManager::DataManager(GameColors* gameColors) : colors(gameColors) {}

void DataManager::ensureDataDirectory() {
    try {
        if(!filesystem::exists(DATA_FOLDER)) {
            filesystem::create_directories(DATA_FOLDER);
        }
    } catch(const exception& e) {
        cout << colors->RED << "Error creating data directory: " << e.what() << colors->RESET << endl;
    }
}

void DataManager::loadUsers() {
    ifstream file(USERS_FILE);
    if(!file.is_open()) return;

    string line;
    while(getline(file, line)) {
        if(!line.empty()) {
            UserProfile user;
            if(user.deserialize(line)) {
                allUsers.push_back(user);
            }
        }
    }
    file.close();
}

void DataManager::saveUsers() {
    ensureDataDirectory();
    ofstream file(USERS_FILE);
    if(!file.is_open()) return;

    for(const auto& user : allUsers) {
        file << user.serialize() << endl;
    }
    file.close();
}

void DataManager::loadLeaderboard() {
    ifstream file(LEADERBOARD_FILE);
    if(!file.is_open()) return;

    string line;
    while(getline(file, line)) {
        if(!line.empty()) {
            LeaderboardEntry entry;
            if(entry.deserialize(line)) {
                leaderboard.push_back(entry);
            }
        }
    }
    file.close();

    sort(leaderboard.begin(), leaderboard.end(),
         [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
             return a.score > b.score;
         });
}

void DataManager::saveLeaderboard() {
    ensureDataDirectory();
    ofstream file(LEADERBOARD_FILE);
    if(!file.is_open()) return;

    for(const auto& entry : leaderboard) {
        file << entry.serialize() << endl;
    }
    file.close();
}

void DataManager::loadUserGameHistory() {
    string historyFile = DATA_FOLDER + "/" + currentUser.username + "_history.dat";
    ifstream file(historyFile);
    if(!file.is_open()) return;

    string line;
    while(getline(file, line)) {
        if(!line.empty()) {
            GameRecord record;
            if(record.deserialize(line)) {
                userGameHistory.push_back(record);
            }
        }
    }
    file.close();
}

void DataManager::saveUserGameHistory() {
    ensureDataDirectory();
    string historyFile = DATA_FOLDER + "/" + currentUser.username + "_history.dat";
    ofstream file(historyFile);
    if(!file.is_open()) return;

    for(const auto& record : userGameHistory) {
        file << record.serialize() << endl;
    }
    file.close();
}

void DataManager::initialize() {
    ensureDataDirectory();
    loadUsers();
    loadLeaderboard();
    loadCurrentUser();
}

bool DataManager::registerUser(const string& username, const string& email) {
    auto it = find_if(allUsers.begin(), allUsers.end(),
                     [&username](const UserProfile& user) {
                         return user.username == username;
                     });

    if(it != allUsers.end()) {
        return false;
    }

    UserProfile newUser(username, email);
    allUsers.push_back(newUser);
    saveUsers();
    return true;
}

bool DataManager::loginUser(const string& username) {
    auto it = find_if(allUsers.begin(), allUsers.end(),
                     [&username](const UserProfile& user) {
                         return user.username == username;
                     });

    if(it != allUsers.end()) {
        currentUser = *it;
        saveCurrentUser();
        loadUserGameHistory();
        return true;
    }
    return false;
}

void DataManager::saveCurrentUser() {
    ensureDataDirectory();
    ofstream file(CURRENT_USER_FILE);
    if(file.is_open()) {
        file << currentUser.username << endl;
        file.close();
    }
}

void DataManager::loadCurrentUser() {
    ifstream file(CURRENT_USER_FILE);
    if(file.is_open()) {
        string username;
        if(getline(file, username)) {
            loginUser(username);
        }
        file.close();
    }
}

void DataManager::recordGameResult(int score, int livesUsed, double duration,
                     const string& difficulty, int powerUpsCollected) {
    if(currentUser.username.empty()) return;

    currentUser.totalGamesPlayed++;
    currentUser.totalScore += score;
    currentUser.totalPlayTime += duration;
    if(score > currentUser.bestScore) {
        currentUser.bestScore = score;
    }

    auto it = find_if(allUsers.begin(), allUsers.end(),
                     [this](UserProfile& user) {
                         return user.username == currentUser.username;
                     });
    if(it != allUsers.end()) {
        *it = currentUser;
    }

    GameRecord record(score, livesUsed, duration, difficulty, powerUpsCollected);
    userGameHistory.push_back(record);

    LeaderboardEntry entry(currentUser.username, score, difficulty);
    leaderboard.push_back(entry);

    sort(leaderboard.begin(), leaderboard.end(),
         [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
             return a.score > b.score;
         });

    if(leaderboard.size() > 20) {
        leaderboard.resize(20);
    }

    saveUsers();
    saveUserGameHistory();
    saveLeaderboard();
}

void DataManager::displayUserProfile() const {
    if(currentUser.username.empty()) {
        cout << colors->RED << "No user logged in!" << colors->RESET << endl;
        return;
    }

    cout << colors->BRIGHT_CYAN << "\n========== USER PROFILE ==========" << colors->RESET << endl;
    cout << colors->YELLOW << "Username: " << colors->WHITE << currentUser.username << endl;
    cout << colors->YELLOW << "Email: " << colors->WHITE << currentUser.email << endl;
    cout << colors->YELLOW << "Member Since: " << colors->WHITE;

    struct tm* timeinfo = localtime(&currentUser.createdDate);
    cout << put_time(timeinfo, "%Y-%m-%d") << endl;

    cout << colors->YELLOW << "Games Played: " << colors->WHITE << currentUser.totalGamesPlayed << endl;
    cout << colors->YELLOW << "Total Score: " << colors->WHITE << currentUser.totalScore << endl;
    cout << colors->YELLOW << "Best Score: " << colors->WHITE << currentUser.bestScore << endl;
    cout << colors->YELLOW << "Total Play Time: " << colors->WHITE
         << (int)(currentUser.totalPlayTime / 60) << " minutes" << endl;
    cout << colors->YELLOW << "Average Score: " << colors->WHITE;

    if(currentUser.totalGamesPlayed > 0) {
        cout << currentUser.totalScore / currentUser.totalGamesPlayed << endl;
    } else {
        cout << "0" << endl;
    }
    cout << colors->BRIGHT_CYAN << "===================================" << colors->RESET << endl;
}

void DataManager::displayLeaderboard() const {
    cout << colors->BRIGHT_CYAN << "\n=============== LEADERBOARD ===============" << colors->RESET << endl;

    cout << colors->YELLOW << left
         << setw(6) << "Rank"
         << setw(16) << "Player"
         << setw(10) << "Score"
         << setw(14) << "Difficulty"
         << setw(12) << "Date" << colors->RESET << endl;

    cout << colors->BRIGHT_BLUE << string(62, '-') << colors->RESET << endl;

    for(size_t i = 0; i < min(leaderboard.size(), (size_t)10); i++) {
        const auto& entry = leaderboard[i];
        struct tm* timeinfo = localtime(&entry.achievedDate);

        string rankColor = colors->WHITE;
        if(i == 0) rankColor = colors->BRIGHT_YELLOW;
        else if(i == 1) rankColor = colors->BRIGHT_WHITE;
        else if(i == 2) rankColor = colors->YELLOW;

        cout << rankColor << left
             << setw(6) << (i + 1)
             << setw(16) << entry.username
             << setw(10) << entry.score
             << setw(14) << entry.difficulty
             << setw(12) << put_time(timeinfo, "%m/%d")
             << colors->RESET << endl;
    }

    cout << colors->BRIGHT_CYAN << string(62, '=') << colors->RESET << endl;

    if(leaderboard.empty()) {
        cout << colors->YELLOW << "No scores recorded yet. Be the first to make it to the leaderboard!" << colors->RESET << endl;
    } else if(leaderboard.size() > 10) {
        cout << colors->CYAN << "Showing top 10 scores. Total recorded scores: " << leaderboard.size() << colors->RESET << endl;
    }
}

void DataManager::displayGameHistory() const {
    if(userGameHistory.empty()) {
        cout << colors->YELLOW << "No game history available." << colors->RESET << endl;
        return;
    }

    cout << colors->BRIGHT_CYAN << "\n=============== GAME HISTORY ===============" << colors->RESET << endl;

    cout << colors->YELLOW << left
         << setw(18) << "Date & Time"
         << setw(8) << "Score"
         << setw(8) << "Lives"
         << setw(12) << "Duration"
         << setw(14) << "Difficulty" << colors->RESET << endl;

    cout << colors->BRIGHT_BLUE << string(60, '-') << colors->RESET << endl;

    size_t startIdx = userGameHistory.size() > 10 ? userGameHistory.size() - 10 : 0;
    for(size_t i = startIdx; i < userGameHistory.size(); i++) {
        const auto& record = userGameHistory[i];
        struct tm* timeinfo = localtime(&record.playDate);

        stringstream dateTimeStream;
        dateTimeStream << put_time(timeinfo, "%m/%d %H:%M");
        string dateTimeStr = dateTimeStream.str();

        stringstream durationStream;
        durationStream << (int)record.playDuration << "s";
        string durationStr = durationStream.str();

        cout << colors->WHITE << left
             << setw(18) << dateTimeStr
             << setw(8) << record.finalScore
             << setw(8) << record.livesUsed
             << setw(12) << durationStr
             << setw(14) << record.difficulty
             << colors->RESET << endl;
    }

    cout << colors->BRIGHT_CYAN << string(60, '=') << colors->RESET << endl;

    if(userGameHistory.size() > 10) {
        cout << colors->CYAN << "Showing last 10 games. Total games played: "
             << userGameHistory.size() << colors->RESET << endl;
    }

    if(!userGameHistory.empty()) {
        int totalScore = 0;
        int totalGames = userGameHistory.size();

        for(const auto& record : userGameHistory) {
            totalScore += record.finalScore;
        }

        double averageScore = (double)totalScore / totalGames;
        cout << colors->YELLOW << "Average Score: " << colors->WHITE
             << fixed << setprecision(1) << averageScore << colors->RESET << endl;
    }
}

const UserProfile& DataManager::getCurrentUser() const { return currentUser; }
bool DataManager::hasLoggedInUser() const { return !currentUser.username.empty(); }

void DataManager::logoutUser() {
    currentUser = UserProfile();
    userGameHistory.clear();
    remove(CURRENT_USER_FILE.c_str());
}

string DataManager::getUserStatsSummary() const {
    if(!hasLoggedInUser()) return "";

    stringstream ss;
    ss << "Player: " << currentUser.username
       << " | Best: " << currentUser.bestScore
       << " | Games: " << currentUser.totalGamesPlayed;
    return ss.str();
}