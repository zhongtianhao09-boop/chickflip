#ifndef SELECTSCENE_H
#define SELECTSCENE_H

#include <QMainWindow>
#include "mymainwindow.h"
class SelectScene : public MyMainWindow
{
    Q_OBJECT
public:
    explicit SelectScene(QWidget *parent = nullptr);
signals:
    void backbtnclicked();
protected:
    void paintEvent(QPaintEvent *evet);
};

#endif // SELECTSCENE_H
