#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mymainwindow.h"
#include "coinbutton.h"
class QLabel;
class QTimer;
class QSoundEffect;
class PlayScene : public MyMainWindow
{
    Q_OBJECT
public:
    explicit PlayScene(int level,QWidget *parent = nullptr);
    static int totalScore();
    void flip(int row,int clo);
    void judgeWin();
signals:
    void backbtnclicked();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void updateHud();
    void setBoardEnabled(bool enabled);
    int mLevel;
    int mElapsedSeconds;
    CoinButton *mCoins[4][4];
    bool mhaswin;
    bool mIsFlipping;
    QLabel *mWinLabel;
    QLabel *mLevelLabel;
    QLabel *mScoreLabel;
    QLabel *mTimerLabel;
    QTimer *mGameTimer;
    QSoundEffect *mFlipSound;
    QSoundEffect *mWinSound;
    bool mResultCommitted;
};

#endif // PLAYSCENE_H
