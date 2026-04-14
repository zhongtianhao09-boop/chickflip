#include "coinbutton.h"
#include <QPainter>
CoinButton::CoinButton(QWidget *parent)
    : QPushButton{parent}
{
    mAnimStep = 0;
    mIsFlipping = false;
    mFlipTimer.setInterval(16);
    connect(&mFlipTimer,&QTimer::timeout,this,&CoinButton::updateFlipFrame);
    this->setState(0);
    this->setStyleSheet("QPushButton{border:0px;}");
};
int CoinButton::state() const
{
    return mState;
};
void CoinButton::setState(int stat)
{
    mState=stat;
    if(mState==1){
        this->setIcon(QIcon(":/res/6.jpg"));
    } else {
        this->setIcon(QIcon(":/res/31.jpg"));
    }
    this->setIconSize(this->size());
};
void CoinButton::paintEvent(QPaintEvent *ev){
    QPainter painter(this);
    static const QPixmap baseSlot(":/res/31.jpg");
    QPixmap pix = baseSlot;
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    QPushButton::paintEvent(ev);
}
void CoinButton::flip(){
    if(mIsFlipping){
        return;
    }
    this->setState(!this->state());
    mIsFlipping = true;
    mAnimStep = 0;
    this->setEnabled(false);
    mFlipTimer.start();
}

void CoinButton::updateFlipFrame(){
    ++mAnimStep;
    const int totalSteps = 12;
    const int halfSteps = totalSteps / 2;
    const int fullWidth = this->width();
    int iconWidth = fullWidth;

    if(mAnimStep <= halfSteps){
        iconWidth = fullWidth - (mAnimStep * fullWidth / halfSteps);
    } else {
        iconWidth = (mAnimStep - halfSteps) * fullWidth / halfSteps;
    }

    if(iconWidth < 1){
        iconWidth = 1;
    }

    this->setIconSize(QSize(iconWidth, this->height()));

    if(mAnimStep >= totalSteps){
        mFlipTimer.stop();
        this->setIconSize(this->size());
        mIsFlipping = false;
        this->setEnabled(true);
    }
}