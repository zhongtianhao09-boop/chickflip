#include "mymainwindow.h"
#include "ui_mymainwindow.h"
#include <QPainter>
MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Find Real Chicken");
    this->setWindowIcon(QIcon(":/res/1.png"));
    this->setFixedSize(320,588);
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
};
void MyMainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix(":/res/20.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

};