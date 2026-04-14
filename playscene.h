#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mymainwindow.h"
#include "coinbutton.h"
class QLabel;
class PlayScene : public MyMainWindow
{
    Q_OBJECT
public:
    explicit PlayScene(int level,QWidget *parent = nullptr);
    void flip(int row,int clo);
    void judgeWin();
signals:
    void backbtnclicked();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
    void setBoardEnabled(bool enabled);
    CoinButton *mCoins[4][4];
    bool mhaswin;
    bool mIsFlipping;
    QLabel *mWinLabel;
};

#endif // PLAYSCENE_H
