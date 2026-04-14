#include "gamesessionmanager.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHash>
#include <algorithm>

bool GameSessionManager::sLoaded = false;
bool GameSessionManager::sActive = false;
QString GameSessionManager::sCurrentUser = "";
int GameSessionManager::sCurrentScore = 0;
int GameSessionManager::sCurrentTimeSeconds = 0;
QVector<RankEntry> GameSessionManager::sEntries;

static QString rankFilePath(){
    return QCoreApplication::applicationDirPath() + "/leaderboard.json";
}

static bool isBetterEntry(const RankEntry &lhs, const RankEntry &rhs){
    if(lhs.score != rhs.score){
        return lhs.score > rhs.score;
    }
    if(lhs.timeSeconds != rhs.timeSeconds){
        return lhs.timeSeconds < rhs.timeSeconds;
    }
    return lhs.userName.toLower() < rhs.userName.toLower();
}

static QVector<RankEntry> deduplicateByUser(const QVector<RankEntry> &entries){
    QHash<QString, RankEntry> bestByUser;
    for(const RankEntry &e : entries){
        const QString key = e.userName.trimmed().toLower();
        if(key.isEmpty()){
            continue;
        }
        if(!bestByUser.contains(key) || isBetterEntry(e, bestByUser.value(key))){
            bestByUser.insert(key, e);
        }
    }
    QVector<RankEntry> merged = bestByUser.values().toVector();
    std::sort(merged.begin(), merged.end(), [](const RankEntry &a, const RankEntry &b){
        return isBetterEntry(a, b);
    });
    return merged;
}

void GameSessionManager::loadLeaderboardIfNeeded(){
    if(sLoaded){
        return;
    }
    sLoaded = true;
    QFile file(rankFilePath());
    if(!file.exists()){
        return;
    }
    if(!file.open(QIODevice::ReadOnly)){
        return;
    }
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    if(!doc.isArray()){
        return;
    }
    const QJsonArray arr = doc.array();
    for(const QJsonValue &v : arr){
        if(!v.isObject()){
            continue;
        }
        const QJsonObject o = v.toObject();
        RankEntry e;
        e.userName = o.value("userName").toString();
        e.score = o.value("score").toInt();
        e.timeSeconds = o.value("timeSeconds").toInt();
        if(e.userName.trimmed().isEmpty()){
            continue;
        }
        sEntries.push_back(e);
    }
    sEntries = deduplicateByUser(sEntries);
}

void GameSessionManager::saveLeaderboard(){
    QJsonArray arr;
    for(const RankEntry &e : sEntries){
        QJsonObject o;
        o.insert("userName", e.userName);
        o.insert("score", e.score);
        o.insert("timeSeconds", e.timeSeconds);
        arr.push_back(o);
    }
    QFile file(rankFilePath());
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        return;
    }
    file.write(QJsonDocument(arr).toJson(QJsonDocument::Indented));
    file.close();
}

void GameSessionManager::beginSession(const QString &userName){
    loadLeaderboardIfNeeded();
    sActive = true;
    sCurrentUser = userName.trimmed();
    sCurrentScore = 0;
    sCurrentTimeSeconds = 0;
}

void GameSessionManager::addLevelResult(int scoreDelta, int timeSecondsDelta){
    if(!sActive){
        return;
    }
    sCurrentScore += std::max(0, scoreDelta);
    sCurrentTimeSeconds += std::max(0, timeSecondsDelta);
}

void GameSessionManager::finishSession(){
    loadLeaderboardIfNeeded();
    if(!sActive || sCurrentUser.isEmpty()){
        sActive = false;
        return;
    }
    if(sCurrentScore > 0 || sCurrentTimeSeconds > 0){
        RankEntry current;
        current.userName = sCurrentUser;
        current.score = sCurrentScore;
        current.timeSeconds = sCurrentTimeSeconds;
        sEntries.push_back(current);
        sEntries = deduplicateByUser(sEntries);
        if(sEntries.size() > 100){
            sEntries = sEntries.mid(0, 100);
        }
        saveLeaderboard();
    }
    sActive = false;
}

int GameSessionManager::currentScore(){
    return sCurrentScore;
}

int GameSessionManager::currentTimeSeconds(){
    return sCurrentTimeSeconds;
}

QString GameSessionManager::currentUser(){
    return sCurrentUser;
}

bool GameSessionManager::hasActiveSession(){
    return sActive;
}

QString GameSessionManager::formatTime(int timeSeconds){
    const int minute = std::max(0, timeSeconds) / 60;
    const int second = std::max(0, timeSeconds) % 60;
    return QString("%1:%2")
        .arg(minute, 2, 10, QLatin1Char('0'))
        .arg(second, 2, 10, QLatin1Char('0'));
}

QVector<RankEntry> GameSessionManager::leaderboard(){
    loadLeaderboardIfNeeded();
    return sEntries;
}
