#include "DataStructures.h"
#include <sstream>
using namespace std;

// UserProfile 用戶檔案實作
// 預設建構函數：初始化所有欄位為預設值
UserProfile::UserProfile() : username(""), email(""), createdDate(0),
               totalGamesPlayed(0), totalScore(0), bestScore(0),
               totalPlayTime(0.0), achievementCount(0) {}

// 參數建構函數：建立新用戶，自動設定創建時間
UserProfile::UserProfile(const string& name, const string& mail)
    : username(name), email(mail), createdDate(time(nullptr)),
      totalGamesPlayed(0), totalScore(0), bestScore(0),
      totalPlayTime(0.0), achievementCount(0) {}

// 序列化：將用戶資料轉換為字串格式（用 | 分隔）
string UserProfile::serialize() const {
    stringstream ss;
    ss << username << "|" << email << "|" << createdDate << "|"
       << totalGamesPlayed << "|" << totalScore << "|" << bestScore << "|"
       << totalPlayTime << "|" << achievementCount;
    return ss.str();
}

// 反序列化：從字串還原用戶資料
bool UserProfile::deserialize(const string& data) {
    stringstream ss(data);
    string item;
    try {
        // 依序讀取每個欄位
        if(!getline(ss, item, '|')) return false;
        username = item;
        if(!getline(ss, item, '|')) return false;
        email = item;
        if(!getline(ss, item, '|')) return false;
        createdDate = stoll(item);          // 字串轉長整數
        if(!getline(ss, item, '|')) return false;
        totalGamesPlayed = stoi(item);      // 字串轉整數
        if(!getline(ss, item, '|')) return false;
        totalScore = stoi(item);
        if(!getline(ss, item, '|')) return false;
        bestScore = stoi(item);
        if(!getline(ss, item, '|')) return false;
        totalPlayTime = stod(item);         // 字串轉雙精度浮點數
        if(!getline(ss, item, '|')) return false;
        achievementCount = stoi(item);
        return true;
    } catch(...) {
        return false;  // 轉換失敗時返回 false
    }
}

// GameRecord 遊戲記錄實作
// 預設建構函數：初始化所有欄位為預設值
GameRecord::GameRecord() : playDate(0), finalScore(0), livesUsed(0),
              playDuration(0.0), difficulty(""), powerUpsCollected(0) {}

// 參數建構函數：記錄新遊戲結果，自動設定遊戲時間
GameRecord::GameRecord(int score, int lives, double duration, const string& diff, int powerups)
    : playDate(time(nullptr)), finalScore(score), livesUsed(lives),
      playDuration(duration), difficulty(diff), powerUpsCollected(powerups) {}

// 序列化：將遊戲記錄轉換為字串格式
string GameRecord::serialize() const {
    stringstream ss;
    ss << playDate << "|" << finalScore << "|" << livesUsed << "|"
       << playDuration << "|" << difficulty << "|" << powerUpsCollected;
    return ss.str();
}

// 反序列化：從字串還原遊戲記錄
bool GameRecord::deserialize(const string& data) {
    stringstream ss(data);
    string item;
    try {
        // 依序讀取每個欄位
        if(!getline(ss, item, '|')) return false;
        playDate = stoll(item);             // 遊戲時間
        if(!getline(ss, item, '|')) return false;
        finalScore = stoi(item);            // 最終分數
        if(!getline(ss, item, '|')) return false;
        livesUsed = stoi(item);             // 使用的生命數
        if(!getline(ss, item, '|')) return false;
        playDuration = stod(item);          // 遊戲時長
        if(!getline(ss, item, '|')) return false;
        difficulty = item;                  // 難度
        if(!getline(ss, item, '|')) return false;
        powerUpsCollected = stoi(item);     // 收集的道具數
        return true;
    } catch(...) {
        return false;  // 轉換失敗時返回 false
    }
}

// LeaderboardEntry 排行榜條目實作
// 預設建構函數：初始化所有欄位為預設值
LeaderboardEntry::LeaderboardEntry() : username(""), score(0), achievedDate(0), difficulty("") {}

// 參數建構函數：建立新排行榜條目，自動設定達成時間
LeaderboardEntry::LeaderboardEntry(const string& name, int sc, const string& diff)
    : username(name), score(sc), achievedDate(time(nullptr)), difficulty(diff) {}

// 序列化：將排行榜條目轉換為字串格式
string LeaderboardEntry::serialize() const {
    stringstream ss;
    ss << username << "|" << score << "|" << achievedDate << "|" << difficulty;
    return ss.str();
}

// 反序列化：從字串還原排行榜條目
bool LeaderboardEntry::deserialize(const string& data) {
    stringstream ss(data);
    string item;
    try {
        // 依序讀取每個欄位
        if(!getline(ss, item, '|')) return false;
        username = item;                    // 用戶名
        if(!getline(ss, item, '|')) return false;
        score = stoi(item);                 // 分數
        if(!getline(ss, item, '|')) return false;
        achievedDate = stoll(item);         // 達成時間
        if(!getline(ss, item, '|')) return false;
        difficulty = item;                  // 難度
        return true;
    } catch(...) {
        return false;  // 轉換失敗時返回 false
    }
}