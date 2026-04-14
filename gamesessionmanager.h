#ifndef GAMESESSIONMANAGER_H
#define GAMESESSIONMANAGER_H

#include <QString>
#include <QVector>

struct RankEntry {
    QString userName;
    int score;
    int timeSeconds;
};

class GameSessionManager
{
public:
    static void beginSession(const QString &userName);
    static void addLevelResult(int scoreDelta, int timeSecondsDelta);
    static void finishSession();

    static int currentScore();
    static int currentTimeSeconds();
    static QString currentUser();
    static bool hasActiveSession();
    static QString formatTime(int timeSeconds);

    static QVector<RankEntry> leaderboard();

private:
    static void loadLeaderboardIfNeeded();
    static void saveLeaderboard();

    static bool sLoaded;
    static bool sActive;
    static QString sCurrentUser;
    static int sCurrentScore;
    static int sCurrentTimeSeconds;
    static QVector<RankEntry> sEntries;
};

#endif // GAMESESSIONMANAGER_H
