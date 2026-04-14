#ifndef COINBUTTON_H
#define COINBUTTON_H

#include <QWidget>
#include <QPushbutton>
#include <QTimer>
class CoinButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CoinButton(QWidget *parent = nullptr);

    int state() const;
    void setState(int state);
    void flip();
signals:
private:
    int mState;
    int mFrame;
    QTimer mTimer;
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // COINBUTTON_H
