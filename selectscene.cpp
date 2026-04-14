#include "selectscene.h"
#include <QPushButton>
#include <QPainter>
#include <QPixmap>
#include "mypushbutton.h"
#include "playscene.h"
SelectScene::SelectScene(QWidget *parent)
    : MyMainWindow{parent}
{
    MyPushButton *btnback=new MyPushButton(":/res/221.jpg",":/res/21.jpg",this);
    btnback->resize(72,32);
    connect(btnback,&QPushButton::clicked,this,&SelectScene::backbtnclicked);
    btnback->move(this->width()-btnback->width(),this->height()-btnback->height());
    const int clowidth=70;
    const int rowHeight=70;
    const int xoffset=25;
    const int yoffset=130;
    for(int i=0;i<20;i++){
        MyPushButton *btn=new MyPushButton(":/res/9.jpg",":/res/9.jpg",this);
        btn->setText(QString::number(i+1));
        btn->setStyleSheet("QPushButton{color:whit;}");
        int row=i/4;
        int col=i%4;
        int x=col*clowidth+xoffset;
        int y=row*rowHeight+yoffset;
        btn->resize(57,57);
        btn->move(x,y);
        connect(btn,&MyPushButton::clicked,[=](){
            PlayScene *playScene=new PlayScene(i+1);
            playScene->setAttribute(Qt::WA_DeleteOnClose);
            playScene->show();
            this->hide();
            connect(playScene,&PlayScene::backbtnclicked,[=](){
                this->show();
                playScene->close();
            });
        });
    };
}
void SelectScene::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix(":/res/20.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
};