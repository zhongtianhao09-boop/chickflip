#ifndef SELECTSCENE_H
#define SELECTSCENE_H

#include <QMainWindow>
#include "mymainwindow.h"
class QLabel;
class SelectScene : public MyMainWindow
{
    Q_OBJECT
public:
    explicit SelectScene(QWidget *parent = nullptr);
signals:
    void backbtnclicked();
protected:
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
private:
    void updateScoreBoard();
    QLabel *mScoreBoard;
};

#endif // SELECTSCENE_H
