#include "selectscene.h"
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QFrame>
#include <QShowEvent>
#include "mypushbutton.h"
#include "playscene.h"
#include "gamesessionmanager.h"

void SelectScene::updateScoreBoard(){
    mScoreBoard->setText(QString("USER %1   SCORE %2   TIME %3")
                             .arg(GameSessionManager::currentUser())
                             .arg(PlayScene::totalScore())
                             .arg(GameSessionManager::formatTime(GameSessionManager::currentTimeSeconds())));
}

SelectScene::SelectScene(QWidget *parent)
    : MyMainWindow{parent}
{
    QFrame *topBar = new QFrame(this);
    topBar->setGeometry(12, 12, this->width()-24, 64);
    topBar->setStyleSheet("QFrame{background:rgba(8,20,40,170);border:1px solid rgba(255,255,255,60);border-radius:12px;}");
    mScoreBoard = new QLabel(topBar);
    mScoreBoard->setGeometry(16, 10, 250, 44);
    mScoreBoard->setStyleSheet("QLabel{color:#F6F8FF;font:700 16px 'Segoe UI';background:transparent;}");
    updateScoreBoard();

    MyPushButton *btnback=new MyPushButton(":/res/221.jpg",":/res/21.jpg",this);
    btnback->resize(72,32);
    connect(btnback,&QPushButton::clicked,this,&SelectScene::backbtnclicked);
    btnback->move(this->width()-btnback->width()-10,this->height()-btnback->height()-10);
    const int clowidth=70;
    const int rowHeight=70;
    const int xoffset=25;
    const int yoffset=110;
    for(int i=0;i<20;i++){
        MyPushButton *btn=new MyPushButton(":/res/9.jpg",":/res/9.jpg",this);
        btn->setText(QString::number(i+1));
        btn->setStyleSheet("QPushButton{color:white;font-weight:bold;}");
        int row=i/4;
        int col=i%4;
        int x=col*clowidth+xoffset;
        int y=row*rowHeight+yoffset;
        btn->resize(57,57);
        btn->move(x,y);
        connect(btn,&MyPushButton::clicked,[=](){
            PlayScene *playScene=new PlayScene(i+1);
            playScene->setAttribute(Qt::WA_DeleteOnClose);
            playScene->move(this->pos());
            playScene->show();
            this->hide();
            connect(playScene,&PlayScene::backbtnclicked,[=](){
                this->move(playScene->pos());
                updateScoreBoard();
                this->show();
                playScene->close();
            });
        });
    };
}

void SelectScene::showEvent(QShowEvent *event){
    MyMainWindow::showEvent(event);
    updateScoreBoard();
}

void SelectScene::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap pix(":/res/20.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
};