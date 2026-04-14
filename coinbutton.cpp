#include "coinbutton.h"
#include <QPainter>
CoinButton::CoinButton(QWidget *parent)
    : QPushButton{parent}
{
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
    QPixmap pix;
    pix.load(":/res/31.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    QPushButton::paintEvent(ev);
}
void CoinButton::flip(){
    this->setState(!this->state());
}