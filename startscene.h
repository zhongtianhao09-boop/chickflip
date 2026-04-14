#ifndef STARTSCENE_H
#define STARTSCENE_H

#include <QMainWindow>
#include "mymainwindow.h"
#include "selectscene.h"
class StartScene : public MyMainWindow
{
    Q_OBJECT
public:
    explicit StartScene(QWidget *parent = nullptr);
private:
    void showLeaderboardDialog();
    SelectScene mSelectScene;
signals:
};

#endif // STARTSCENE_H
