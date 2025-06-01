#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <string>
#include <ctime>
using namespace std;

struct UserProfile {
    string username;
    string email;
    time_t createdDate;
    int totalGamesPlayed;
    int totalScore;
    int bestScore;
    double totalPlayTime;
    int achievementCount;

    UserProfile();
    UserProfile(const string& name, const string& mail);
    string serialize() const;
    bool deserialize(const string& data);
};

struct GameRecord {
    time_t playDate;
    int finalScore;
    int livesUsed;
    double playDuration;
    string difficulty;
    int powerUpsCollected;

    GameRecord();
    GameRecord(int score, int lives, double duration, const string& diff, int powerups);
    string serialize() const;
    bool deserialize(const string& data);
};

struct LeaderboardEntry {
    string username;
    int score;
    time_t achievedDate;
    string difficulty;

    LeaderboardEntry();
    LeaderboardEntry(const string& name, int sc, const string& diff);
    string serialize() const;
    bool deserialize(const string& data);
};

#endif // DATASTRUCTURES_H