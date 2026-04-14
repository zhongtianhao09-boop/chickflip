#include "mypushbutton.h"
#include <QPainter>
#include <QPropertyAnimation>
MyPushButton::MyPushButton(QString normalImg,QString pressedImg,QWidget *parent)
    : QPushButton(parent)
    ,mNormalImg(normalImg)
    ,mPressedImg(pressedImg){
    mstate=Normal;
};
void MyPushButton::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    QPixmap pix;
    if(mstate==Pressed){
        pix.load(mPressedImg);
    } else {
        pix.load(mNormalImg);
    }
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    painter.drawText(0,0,this->width(),this->height(),Qt::AlignHCenter|Qt::AlignVCenter,this->text());
};
void MyPushButton::movedown(){
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry",this);
    animation->setStartValue(this->geometry());
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation->setDuration(100);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
};
void MyPushButton::moveup(){
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry",this);
    animation->setStartValue(this->geometry());
    animation->setEndValue(QRect(this->x(),this->y()-10,this->width(),this->height()));
    animation->setDuration(100);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
};
void MyPushButton::mousePressEvent(QMouseEvent *e){
    this->mstate=Pressed;
    update();
    QPushButton::mousePressEvent(e);
};
void MyPushButton::mouseReleaseEvent(QMouseEvent *e){
    this->mstate=Normal;
    update();
    QPushButton::mouseReleaseEvent(e);
};