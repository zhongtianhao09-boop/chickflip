#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mymainwindow.h"
#include "coinbutton.h"
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
    void paintEvent(QPaintEvent *event);
private:
    CoinButton *mCoins[4][4];
    bool mhaswin;
};

#endif // PLAYSCENE_H
