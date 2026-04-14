#include "playscene.h"
#include "mypushbutton.h"
#include "gamesessionmanager.h"
#include <QPainter>
#include <QLabel>
#include "coinbutton.h"
#include "dataconfig.h"
#include <QTimer>
#include <QPropertyAnimation>
#include <QFrame>
#include <QSoundEffect>
#include <QUrl>

int PlayScene::totalScore(){
    return GameSessionManager::currentScore();
}

void PlayScene::updateHud(){
    mLevelLabel->setText(QString("LEVEL %1").arg(mLevel));
    mScoreLabel->setText(QString("SCORE %1").arg(GameSessionManager::currentScore()));
    mTimerLabel->setText(QString("TIME %1")
                             .arg(GameSessionManager::formatTime(GameSessionManager::currentTimeSeconds() + mElapsedSeconds)));
}

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
    if(mFlipSound){
        mFlipSound->play();
    }
    this->mCoins[row][clo]->flip();
    QTimer::singleShot(220, this, [this, row, clo](){
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
    mGameTimer->stop();
    if(mWinSound){
        mWinSound->play();
    }
    if(!mResultCommitted){
        GameSessionManager::addLevelResult(5, mElapsedSeconds);
        mResultCommitted = true;
    }
    updateHud();
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
    QTimer::singleShot(2000, this, [this](){
        emit backbtnclicked();
    });
};
PlayScene::PlayScene(int level,QWidget *parent)
    : MyMainWindow{parent}
{
    mLevel = level;
    mElapsedSeconds = 0;
    mhaswin=false;
    mIsFlipping=false;
    mWinLabel=nullptr;
    mLevelLabel=nullptr;
    mScoreLabel=nullptr;
    mTimerLabel=nullptr;
    mGameTimer=new QTimer(this);
    mFlipSound = new QSoundEffect(this);
    mFlipSound->setSource(QUrl("qrc:/res/xm2951.wav"));
    mFlipSound->setLoopCount(1);
    mFlipSound->setVolume(0.35f);
    mWinSound = new QSoundEffect(this);
    mWinSound->setSource(QUrl("qrc:/res/9734.wav"));
    mWinSound->setLoopCount(1);
    mWinSound->setVolume(0.55f);
    mResultCommitted = false;

    QFrame *topBar = new QFrame(this);
    topBar->setGeometry(12, 12, this->width()-24, 70);
    topBar->setStyleSheet("QFrame{background:rgba(8,20,40,170);border:1px solid rgba(255,255,255,60);border-radius:12px;}");

    mLevelLabel = new QLabel(topBar);
    mScoreLabel = new QLabel(topBar);
    mTimerLabel = new QLabel(topBar);
    mLevelLabel->setGeometry(12, 14, 86, 42);
    mScoreLabel->setGeometry(102, 14, 98, 42);
    mTimerLabel->setGeometry(204, 14, 80, 42);
    const QString hudStyle = "QLabel{color:#F6F8FF;font:700 12px 'Segoe UI';background:transparent;}";
    mLevelLabel->setStyleSheet(hudStyle);
    mScoreLabel->setStyleSheet(hudStyle);
    mTimerLabel->setStyleSheet(hudStyle);
    mLevelLabel->setAlignment(Qt::AlignCenter);
    mScoreLabel->setAlignment(Qt::AlignCenter);
    mTimerLabel->setAlignment(Qt::AlignCenter);

    connect(mGameTimer,&QTimer::timeout,this,[this](){
        ++mElapsedSeconds;
        updateHud();
    });
    mGameTimer->start(1000);

    updateHud();

    MyPushButton *btnback=new MyPushButton(":/res/221.jpg",":/res/21.jpg",this);
    btnback->resize(72,32);
    connect(btnback,&MyPushButton::clicked,this,[this](){
        mGameTimer->stop();
        if(!mResultCommitted){
            GameSessionManager::addLevelResult(0, mElapsedSeconds);
            mResultCommitted = true;
        }
        emit backbtnclicked();
    });
    btnback->move(this->width()-btnback->width()-10,this->height()-btnback->height()-10);
    dataConfig data;
    const int safeLevel = data.mData.contains(level) ? level : 1;
    mLevel = safeLevel;
    updateHud();
    const int clowidth=50;
    const int rowHeight=50;
    const int xoffset=57;
    const int yoffset=220;
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