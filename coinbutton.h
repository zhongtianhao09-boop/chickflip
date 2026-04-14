#ifndef COINBUTTON_H
#define COINBUTTON_H

#include <QWidget>
#include <QPushButton>
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
    void updateFlipFrame();
    int mState;
    int mAnimStep;
    bool mIsFlipping;
    QTimer mFlipTimer;
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // COINBUTTON_H
