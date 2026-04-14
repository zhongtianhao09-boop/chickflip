#include "playscene.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QLabel>
#include "coinbutton.h"
#include "dataconfig.h"
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QPropertyAnimation>
void PlayScene::flip(int row,int clo){
    if(mhaswin){
        return;
    }
    this->mCoins[row][clo]->flip();
    QTimer::singleShot(30,[=](){
    if(row+1<4){
        this->mCoins[row+1][clo]->flip();}
    if(row-1>=0){
        this->mCoins[row-1][clo]->flip();}
    if(clo+1<4){
        this->mCoins[row][clo+1]->flip();}
    if(clo-1>=0){
        this->mCoins[row][clo-1]->flip();}
      this->judgeWin();
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
    QPixmap pix = QPixmap(":/res/7.jpg");
    qreal scale = 0.5; // 0.5 = 50%大小，0.3=30%，0.6=60%，直接改这个数
    QPixmap scaledPix = pix.scaled(
        pix.width() * scale,
        pix.height() * scale,
        Qt::KeepAspectRatio,       // 保持宽高比，字体不变形
        Qt::SmoothTransformation  // 平滑缩放，保证字体清晰
        );
    mhaswin=true;
    // 2. 给 labelWin 设置缩放后的图片，同步大小
    QLabel *labelWin = new QLabel(this);
    labelWin->setPixmap(scaledPix);
    labelWin->resize(scaledPix.size()); // 标签大小 = 缩放后图片大小

    // 3. 修正动画位置（适配缩小后的尺寸，保证居中）
    int startY = -scaledPix.height();
    int endY = this->height() / 2 - scaledPix.height() / 2;
    labelWin->move(this->width() / 2 - scaledPix.width() / 2, startY);
    labelWin->show();
    labelWin->raise(); // 置顶，不被按钮挡住

    // 4. 动画代码（同步缩放后的尺寸）
    QPropertyAnimation *animation = new QPropertyAnimation(labelWin, "geometry", this);
    animation->setDuration(500); // 0.5秒动画
    animation->setStartValue(labelWin->geometry());
    animation->setEndValue(QRect(
        this->width() / 2 - scaledPix.width() / 2,
        endY,
        scaledPix.width(),
        scaledPix.height()
        ));
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    connect(animation, &QPropertyAnimation::finished,[=](){
        QTimer::singleShot(2000, labelWin, &QLabel::deleteLater);
    });
};
PlayScene::PlayScene(int level,QWidget *parent)
    : MyMainWindow{parent}
{   mhaswin=false;
    MyPushButton *btnback=new MyPushButton(":/res/221.jpg",":/res/21.jpg",this);
    btnback->resize(72,32);
    connect(btnback,&MyPushButton::clicked,this,&PlayScene::backbtnclicked);
    btnback->move(this->width()-btnback->width(),this->height()-btnback->height());
    QLabel *label=new QLabel(this);
    label->setText(QString("Level: %1").arg(level));
    label->move(30,this->height()-label->height());
    label->setFont(QFont("华文新魏",20));
    label->resize(150,50);
    const int clowidth=50;
    const int rowHeight=50;
    const int xoffset=57;
    const int yoffset=200;
    dataConfig data;
    QVector <QVector<int>>dataArray=data.mData[level];
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
    QPainter painter(this);
    QPixmap pix(":/res/20.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
};