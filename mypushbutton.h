#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    enum MyPushButtonState{
      Normal,
      Pressed
    };
    MyPushButton(QString normalImg,QString pressedImg,QWidget *parent = nullptr);
    void moveup();
    void movedown();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
signals:
private:
    //正常图片
    //鼠标按下图片
    QString mNormalImg;
    QString mPressedImg;
    MyPushButtonState mstate;
};

#endif // MYPUSHBUTTON_H
