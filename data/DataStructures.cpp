#include "DataStructures.h"
#include <sstream>
using namespace std;

// UserProfile implementation
UserProfile::UserProfile() : username(""), email(""), createdDate(0),
               totalGamesPlayed(0), totalScore(0), bestScore(0),
               totalPlayTime(0.0), achievementCount(0) {}

UserProfile::UserProfile(const string& name, const string& mail)
    : username(name), email(mail), createdDate(time(nullptr)),
      totalGamesPlayed(0), totalScore(0), bestScore(0),
      totalPlayTime(0.0), achievementCount(0) {}

string UserProfile::serialize() const {
    stringstream ss;
    ss << username << "|" << email << "|" << createdDate << "|"
       << totalGamesPlayed << "|" << totalScore << "|" << bestScore << "|"
       << totalPlayTime << "|" << achievementCount;
    return ss.str();
}

bool UserProfile::deserialize(const string& data) {
    stringstream ss(data);
    string item;
    try {
        if(!getline(ss, item, '|')) return false;
        username = item;
        if(!getline(ss, item, '|')) return false;
        email = item;
        if(!getline(ss, item, '|')) return false;
        createdDate = stoll(item);
        if(!getline(ss, item, '|')) return false;
        totalGamesPlayed = stoi(item);
        if(!getline(ss, item, '|')) return false;
        totalScore = stoi(item);
        if(!getline(ss, item, '|')) return false;
        bestScore = stoi(item);
        if(!getline(ss, item, '|')) return false;
        totalPlayTime = stod(item);
        if(!getline(ss, item, '|')) return false;
        achievementCount = stoi(item);
        return true;
    } catch(...) {
        return false;
    }
}

// GameRecord implementation
GameRecord::GameRecord() : playDate(0), finalScore(0), livesUsed(0),
              playDuration(0.0), difficulty(""), powerUpsCollected(0) {}

GameRecord::GameRecord(int score, int lives, double duration, const string& diff, int powerups)
    : playDate(time(nullptr)), finalScore(score), livesUsed(lives),
      playDuration(duration), difficulty(diff), powerUpsCollected(powerups) {}

string GameRecord::serialize() const {
    stringstream ss;
    ss << playDate << "|" << finalScore << "|" << livesUsed << "|"
       << playDuration << "|" << difficulty << "|" << powerUpsCollected;
    return ss.str();
}

bool GameRecord::deserialize(const string& data) {
    stringstream ss(data);
    string item;
    try {
        if(!getline(ss, item, '|')) return false;
        playDate = stoll(item);
        if(!getline(ss, item, '|')) return false;
        finalScore = stoi(item);
        if(!getline(ss, item, '|')) return false;
        livesUsed = stoi(item);
        if(!getline(ss, item, '|')) return false;
        playDuration = stod(item);
        if(!getline(ss, item, '|')) return false;
        difficulty = item;
        if(!getline(ss, item, '|')) return false;
        powerUpsCollected = stoi(item);
        return true;
    } catch(...) {
        return false;
    }
}

// LeaderboardEntry implementation
LeaderboardEntry::LeaderboardEntry() : username(""), score(0), achievedDate(0), difficulty("") {}

LeaderboardEntry::LeaderboardEntry(const string& name, int sc, const string& diff)
    : username(name), score(sc), achievedDate(time(nullptr)), difficulty(diff) {}

string LeaderboardEntry::serialize() const {
    stringstream ss;
    ss << username << "|" << score << "|" << achievedDate << "|" << difficulty;
    return ss.str();
}

bool LeaderboardEntry::deserialize(const string& data) {
    stringstream ss(data);
    string item;
    try {
        if(!getline(ss, item, '|')) return false;
        username = item;
        if(!getline(ss, item, '|')) return false;
        score = stoi(item);
        if(!getline(ss, item, '|')) return false;
        achievedDate = stoll(item);
        if(!getline(ss, item, '|')) return false;
        difficulty = item;
        return true;
    } catch(...) {
        return false;
    }
}