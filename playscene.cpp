#include "playscene.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QLabel>
#include "coinbutton.h"
#include "dataconfig.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QMap>
void PlayScene::setBoardEnabled(bool enabled){
    for(int row=0;row<4;row++){
        for(int col=0;col<4;col++){
            mCoins[row][col]->setEnabled(enabled);
        }
    }
}

void PlayScene::flip(int row,int clo){
    if(mhaswin || mIsFlipping){
        return;
    }
    mIsFlipping = true;
    setBoardEnabled(false);
    this->mCoins[row][clo]->flip();
    QTimer::singleShot(220,[=](){
    if(row+1<4){
        this->mCoins[row+1][clo]->flip();}
    if(row-1>=0){
        this->mCoins[row-1][clo]->flip();}
    if(clo+1<4){
        this->mCoins[row][clo+1]->flip();}
    if(clo-1>=0){
        this->mCoins[row][clo-1]->flip();}
      this->judgeWin();
      if(!mhaswin){
          mIsFlipping = false;
          setBoardEnabled(true);
      }
    });
};
void PlayScene::judgeWin(){
    for(int row=0;row<4;row++){
        for(int clo=0;clo<4;clo++){
            if(!this->mCoins[row][clo]->state()){
                return;
            }
        }
    };
    static const QPixmap pix(":/res/7.jpg");
    qreal scale = 0.5;
    QPixmap scaledPix = pix.scaled(
        pix.width() * scale,
        pix.height() * scale,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    mhaswin=true;
    if (!mWinLabel) {
        mWinLabel = new QLabel(this);
    }
    mWinLabel->setPixmap(scaledPix);
    mWinLabel->resize(scaledPix.size());
    int startY = -scaledPix.height();
    int endY = this->height() / 2 - scaledPix.height() / 2;
    mWinLabel->move(this->width() / 2 - scaledPix.width() / 2, startY);
    mWinLabel->show();
    mWinLabel->raise();

    QPropertyAnimation *animation = new QPropertyAnimation(mWinLabel, "geometry", this);
    animation->setDuration(500);
    animation->setStartValue(mWinLabel->geometry());
    animation->setEndValue(QRect(
        this->width() / 2 - scaledPix.width() / 2,
        endY,
        scaledPix.width(),
        scaledPix.height()
        ));
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
};
PlayScene::PlayScene(int level,QWidget *parent)
    : MyMainWindow{parent}
{
    mhaswin=false;
    mIsFlipping=false;
    mWinLabel=nullptr;
    MyPushButton *btnback=new MyPushButton(":/res/221.jpg",":/res/21.jpg",this);
    btnback->resize(72,32);
    connect(btnback,&MyPushButton::clicked,this,&PlayScene::backbtnclicked);
    btnback->move(this->width()-btnback->width(),this->height()-btnback->height());
    QLabel *label=new QLabel(this);
    label->setText(QString("Level: %1").arg(safeLevel));
    label->move(30,this->height()-label->height());
    label->setFont(QFont("华文新魏",20));
    label->resize(150,50);
    const int clowidth=50;
    const int rowHeight=50;
    const int xoffset=57;
    const int yoffset=200;
    dataConfig data;
    const int safeLevel = data.mData.contains(level) ? level : 1;
    QVector <QVector<int>>dataArray=data.mData[safeLevel];
    for(int row=0;row<4;row++){
        for(int col=0;col<4;col++){
            CoinButton *btn=new CoinButton(this);
            mCoins[row][col]=btn;
            int x=col*clowidth+xoffset;
            int y=row*rowHeight+yoffset;
            btn->setGeometry(x,y,50,50);
            btn->setState(dataArray[row][col]);
            connect(btn,&CoinButton::clicked,[=](){
                this->flip(row,col);
            });
        }
    }
};
void PlayScene::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    static const QPixmap pix(":/res/20.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
};