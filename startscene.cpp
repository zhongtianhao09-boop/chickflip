#include "startscene.h"
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include "mypushbutton.h"
StartScene::StartScene(QWidget *parent)
    : MyMainWindow{parent}
{
    this->setWindowTitle("chickflip");
    MyPushButton *startbtn= new MyPushButton(":/res/14.jpg",":/res/14.jpg",this);
    startbtn->resize(114,114);
    startbtn->move(this->width()/2-startbtn->width()/2,this->height()*3/4-startbtn->height()/2);
    connect(&this->mSelectScene,&SelectScene::backbtnclicked,[=](){
        this->show();
        this->mSelectScene.hide();
    });
    connect(startbtn,&QPushButton::clicked,[=](){
        startbtn->movedown();
        QTimer::singleShot(150,[=](){
        startbtn->setEnabled(false);
        startbtn->moveup();
        });
        QTimer::singleShot(300,[=](){
            startbtn->setEnabled(true);
            this->hide();
            this->mSelectScene.show();
            this->mSelectScene.move(this->pos());
        });
    });
}
