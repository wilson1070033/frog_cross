#include "DataManager.h"
#include "../core/GameColors.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <filesystem>
using namespace std;

// 建構函數：初始化顏色系統
DataManager::DataManager(GameColors* gameColors) : colors(gameColors) {}

// 確保資料目錄存在，不存在就創建
void DataManager::ensureDataDirectory() {
    try {
        if(!filesystem::exists(DATA_FOLDER)) {
            filesystem::create_directories(DATA_FOLDER);
        }
    } catch(const exception& e) {
        cout << colors->RED << "Error creating data directory: " << e.what() << colors->RESET << endl;
    }
}

// 從檔案載入所有用戶資料
void DataManager::loadUsers() {
    ifstream file(USERS_FILE);
    if(!file.is_open()) return;

    string line;
    while(getline(file, line)) {
        if(!line.empty()) {
            UserProfile user;
            if(user.deserialize(line)) {  // 反序列化用戶資料
                allUsers.push_back(user);
            }
        }
    }
    file.close();
}

// 將所有用戶資料保存到檔案
void DataManager::saveUsers() {
    ensureDataDirectory();
    ofstream file(USERS_FILE);
    if(!file.is_open()) return;

    for(const auto& user : allUsers) {
        file << user.serialize() << endl;  // 序列化用戶資料
    }
    file.close();
}

// 載入排行榜資料並按分數排序
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

    // 按分數由高到低排序
    sort(leaderboard.begin(), leaderboard.end(),
         [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
             return a.score > b.score;
         });
}

// 保存排行榜資料到檔案
void DataManager::saveLeaderboard() {
    ensureDataDirectory();
    ofstream file(LEADERBOARD_FILE);
    if(!file.is_open()) return;

    for(const auto& entry : leaderboard) {
        file << entry.serialize() << endl;
    }
    file.close();
}

// 載入當前用戶的遊戲歷史記錄
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

// 保存當前用戶的遊戲歷史記錄
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

// 初始化資料管理器：載入所有資料
void DataManager::initialize() {
    ensureDataDirectory();
    loadUsers();
    loadLeaderboard();
    loadCurrentUser();
}

// 註冊新用戶：檢查用戶名是否已存在
bool DataManager::registerUser(const string& username, const string& email) {
    // 查找是否已有相同用戶名
    auto it = find_if(allUsers.begin(), allUsers.end(),
                     [&username](const UserProfile& user) {
                         return user.username == username;
                     });

    if(it != allUsers.end()) {
        return false;  // 用戶名已存在
    }

    // 創建新用戶並保存
    UserProfile newUser(username, email);
    allUsers.push_back(newUser);
    saveUsers();
    return true;
}

// 用戶登入：查找用戶並設為當前用戶
bool DataManager::loginUser(const string& username) {
    auto it = find_if(allUsers.begin(), allUsers.end(),
                     [&username](const UserProfile& user) {
                         return user.username == username;
                     });

    if(it != allUsers.end()) {
        currentUser = *it;
        saveCurrentUser();       // 保存當前登入用戶
        loadUserGameHistory();   // 載入該用戶的遊戲歷史
        return true;
    }
    return false;
}

// 保存當前登入的用戶資訊
void DataManager::saveCurrentUser() {
    ensureDataDirectory();
    ofstream file(CURRENT_USER_FILE);
    if(file.is_open()) {
        file << currentUser.username << endl;
        file.close();
    }
}

// 載入上次登入的用戶
void DataManager::loadCurrentUser() {
    ifstream file(CURRENT_USER_FILE);
    if(file.is_open()) {
        string username;
        if(getline(file, username)) {
            loginUser(username);  // 自動登入上次的用戶
        }
        file.close();
    }
}

// 記錄遊戲結果：更新用戶統計和排行榜
void DataManager::recordGameResult(int score, int livesUsed, double duration,
                     const string& difficulty, int powerUpsCollected) {
    if(currentUser.username.empty()) return;  // 沒有登入用戶就不記錄

    // 更新當前用戶統計
    currentUser.totalGamesPlayed++;
    currentUser.totalScore += score;
    currentUser.totalPlayTime += duration;
    if(score > currentUser.bestScore) {
        currentUser.bestScore = score;
    }

    // 更新用戶列表中的資料
    auto it = find_if(allUsers.begin(), allUsers.end(),
                     [this](UserProfile& user) {
                         return user.username == currentUser.username;
                     });
    if(it != allUsers.end()) {
        *it = currentUser;
    }

    // 添加遊戲記錄
    GameRecord record(score, livesUsed, duration, difficulty, powerUpsCollected);
    userGameHistory.push_back(record);

    // 添加到排行榜
    LeaderboardEntry entry(currentUser.username, score, difficulty);
    leaderboard.push_back(entry);

    // 重新排序排行榜
    sort(leaderboard.begin(), leaderboard.end(),
         [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
             return a.score > b.score;
         });

    // 保留前20名
    if(leaderboard.size() > 20) {
        leaderboard.resize(20);
    }

    // 保存所有資料
    saveUsers();
    saveUserGameHistory();
    saveLeaderboard();
}

// 顯示用戶個人檔案資訊
void DataManager::displayUserProfile() const {
    if(currentUser.username.empty()) {
        cout << colors->RED << "No user logged in!" << colors->RESET << endl;
        return;
    }

    cout << colors->BRIGHT_CYAN << "\n========== USER PROFILE ==========" << colors->RESET << endl;
    cout << colors->YELLOW << "Username: " << colors->WHITE << currentUser.username << endl;
    cout << colors->YELLOW << "Email: " << colors->WHITE << currentUser.email << endl;
    cout << colors->YELLOW << "Member Since: " << colors->WHITE;

    // 格式化顯示註冊日期
    struct tm* timeinfo = localtime(&currentUser.createdDate);
    cout << put_time(timeinfo, "%Y-%m-%d") << endl;

    cout << colors->YELLOW << "Games Played: " << colors->WHITE << currentUser.totalGamesPlayed << endl;
    cout << colors->YELLOW << "Total Score: " << colors->WHITE << currentUser.totalScore << endl;
    cout << colors->YELLOW << "Best Score: " << colors->WHITE << currentUser.bestScore << endl;
    cout << colors->YELLOW << "Total Play Time: " << colors->WHITE
         << (int)(currentUser.totalPlayTime / 60) << " minutes" << endl;
    cout << colors->YELLOW << "Average Score: " << colors->WHITE;

    // 計算平均分數
    if(currentUser.totalGamesPlayed > 0) {
        cout << currentUser.totalScore / currentUser.totalGamesPlayed << endl;
    } else {
        cout << "0" << endl;
    }
    cout << colors->BRIGHT_CYAN << "===================================" << colors->RESET << endl;
}

// 顯示排行榜（前10名）
void DataManager::displayLeaderboard() const {
    cout << colors->BRIGHT_CYAN << "\n=============== LEADERBOARD ===============" << colors->RESET << endl;

    // 顯示表頭
    cout << colors->YELLOW << left
         << setw(6) << "Rank"
         << setw(16) << "Player"
         << setw(10) << "Score"
         << setw(14) << "Difficulty"
         << setw(12) << "Date" << colors->RESET << endl;

    cout << colors->BRIGHT_BLUE << string(62, '-') << colors->RESET << endl;

    // 顯示前10名
    for(size_t i = 0; i < min(leaderboard.size(), (size_t)10); i++) {
        const auto& entry = leaderboard[i];
        struct tm* timeinfo = localtime(&entry.achievedDate);

        // 前三名使用特殊顏色
        string rankColor = colors->WHITE;
        if(i == 0) rankColor = colors->BRIGHT_YELLOW;       // 金色
        else if(i == 1) rankColor = colors->BRIGHT_WHITE;   // 銀色
        else if(i == 2) rankColor = colors->YELLOW;         // 銅色

        cout << rankColor << left
             << setw(6) << (i + 1)
             << setw(16) << entry.username
             << setw(10) << entry.score
             << setw(14) << entry.difficulty
             << setw(12) << put_time(timeinfo, "%m/%d")
             << colors->RESET << endl;
    }

    cout << colors->BRIGHT_CYAN << string(62, '=') << colors->RESET << endl;

    // 顯示額外資訊
    if(leaderboard.empty()) {
        cout << colors->YELLOW << "No scores recorded yet. Be the first to make it to the leaderboard!" << colors->RESET << endl;
    } else if(leaderboard.size() > 10) {
        cout << colors->CYAN << "Showing top 10 scores. Total recorded scores: " << leaderboard.size() << colors->RESET << endl;
    }
}

// 顯示用戶遊戲歷史（最近10場）
void DataManager::displayGameHistory() const {
    if(userGameHistory.empty()) {
        cout << colors->YELLOW << "No game history available." << colors->RESET << endl;
        return;
    }

    cout << colors->BRIGHT_CYAN << "\n=============== GAME HISTORY ===============" << colors->RESET << endl;

    // 顯示表頭
    cout << colors->YELLOW << left
         << setw(18) << "Date & Time"
         << setw(8) << "Score"
         << setw(8) << "Lives"
         << setw(12) << "Duration"
         << setw(14) << "Difficulty" << colors->RESET << endl;

    cout << colors->BRIGHT_BLUE << string(60, '-') << colors->RESET << endl;

    // 顯示最近10場遊戲
    size_t startIdx = userGameHistory.size() > 10 ? userGameHistory.size() - 10 : 0;
    for(size_t i = startIdx; i < userGameHistory.size(); i++) {
        const auto& record = userGameHistory[i];
        struct tm* timeinfo = localtime(&record.playDate);

        // 格式化日期時間
        stringstream dateTimeStream;
        dateTimeStream << put_time(timeinfo, "%m/%d %H:%M");
        string dateTimeStr = dateTimeStream.str();

        // 格式化遊戲時長
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

    // 顯示統計資訊
    if(userGameHistory.size() > 10) {
        cout << colors->CYAN << "Showing last 10 games. Total games played: "
             << userGameHistory.size() << colors->RESET << endl;
    }

    if(!userGameHistory.empty()) {
        // 計算平均分數
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

// Getter 函數群
const UserProfile& DataManager::getCurrentUser() const { return currentUser; }
bool DataManager::hasLoggedInUser() const { return !currentUser.username.empty(); }

// 登出用戶：清除當前用戶資料
void DataManager::logoutUser() {
    currentUser = UserProfile();        // 重置當前用戶
    userGameHistory.clear();            // 清除遊戲歷史
    remove(CURRENT_USER_FILE.c_str());  // 刪除登入檔案
}

// 獲取用戶統計摘要字串
string DataManager::getUserStatsSummary() const {
    if(!hasLoggedInUser()) return "";

    stringstream ss;
    ss << "Player: " << currentUser.username
       << " | Best: " << currentUser.bestScore
       << " | Games: " << currentUser.totalGamesPlayed;
    return ss.str();
}