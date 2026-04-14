#include "startscene.h"
#include "gamesessionmanager.h"
#include <QPushButton>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include "mypushbutton.h"

void StartScene::showLeaderboardDialog(){
    QDialog dialog(this);
    dialog.setWindowTitle("Leaderboard");
    dialog.resize(420, 520);
    dialog.setStyleSheet("QDialog{background:#0f1b31;color:#f6f8ff;}");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QTableWidget *table = new QTableWidget(&dialog);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({"Rank", "Player", "Score", "Time"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setStyleSheet("QTableWidget{background:#142744;border:1px solid #38547d;color:#f6f8ff;gridline-color:#294466;}");
    const QVector<RankEntry> entries = GameSessionManager::leaderboard();
    table->setRowCount(entries.size());
    for(int i=0;i<entries.size();++i){
        const RankEntry &e = entries[i];
        table->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        table->setItem(i,1,new QTableWidgetItem(e.userName));
        table->setItem(i,2,new QTableWidgetItem(QString::number(e.score)));
        table->setItem(i,3,new QTableWidgetItem(GameSessionManager::formatTime(e.timeSeconds)));
    }
    layout->addWidget(table);
    dialog.exec();
}

StartScene::StartScene(QWidget *parent)
    : MyMainWindow{parent}
{
    this->setWindowTitle("chickflip");
    QLabel *title = new QLabel("COINFLIP", this);
    title->setGeometry(0, 70, this->width(), 56);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("QLabel{color:#F8FBFF;font:900 36px 'Segoe UI';letter-spacing:2px;background:transparent;}");

    QLabel *subtitle = new QLabel("Flip coins. Beat all levels.", this);
    subtitle->setGeometry(0, 126, this->width(), 24);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("QLabel{color:rgba(248,251,255,190);font:600 13px 'Segoe UI';background:transparent;}");

    QFrame *rankPanel = new QFrame(this);
    rankPanel->setGeometry(this->width()/2-110, 220, 220, 122);
    rankPanel->setStyleSheet("QFrame{background:rgba(10,26,50,165);border:1px solid rgba(255,255,255,70);border-radius:14px;}");
    auto *shadow = new QGraphicsDropShadowEffect(rankPanel);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 6);
    shadow->setColor(QColor(0, 0, 0, 120));
    rankPanel->setGraphicsEffect(shadow);

    MyPushButton *rankBtn = new MyPushButton(":/res/9.jpg",":/res/9.jpg",this);
    rankBtn->setText("RANK");
    rankBtn->setGeometry(this->width()/2-34, 232, 68, 68);
    rankBtn->setStyleSheet("QPushButton{color:white;font:800 12px 'Segoe UI';}");
    connect(rankBtn,&QPushButton::clicked,this,[this](){
        showLeaderboardDialog();
    });
    QLabel *rankHint = new QLabel("Leaderboard", this);
    rankHint->setGeometry(this->width()/2-70, 302, 140, 22);
    rankHint->setAlignment(Qt::AlignCenter);
    rankHint->setStyleSheet("QLabel{color:#EAF2FF;font:600 13px 'Segoe UI';background:transparent;}");

    MyPushButton *startbtn= new MyPushButton(":/res/14.jpg",":/res/14.jpg",this);
    startbtn->resize(114,114);
    startbtn->move(this->width()/2-startbtn->width()/2,this->height()*3/4-startbtn->height()/2);
    connect(&this->mSelectScene,&SelectScene::backbtnclicked,[=](){
        GameSessionManager::finishSession();
        this->show();
        this->mSelectScene.hide();
    });
    connect(startbtn,&QPushButton::clicked,[=](){
        QDialog nameDialog(this);
        nameDialog.setWindowTitle("Player Name");
        nameDialog.resize(360, 190);
        nameDialog.setStyleSheet("QDialog{background:#0f1b31;color:#f6f8ff;}");
        QVBoxLayout *nameLayout = new QVBoxLayout(&nameDialog);
        nameLayout->setContentsMargins(18, 18, 18, 16);
        nameLayout->setSpacing(12);
        QLabel *nameTitle = new QLabel("Enter your username", &nameDialog);
        nameTitle->setStyleSheet("QLabel{font:700 16px 'Segoe UI';color:#f6f8ff;}");
        QLabel *nameSub = new QLabel("No password required. Rankings are based on score and time.", &nameDialog);
        nameSub->setWordWrap(true);
        nameSub->setStyleSheet("QLabel{font:500 12px 'Segoe UI';color:rgba(246,248,255,190);}");
        QLineEdit *nameEdit = new QLineEdit(&nameDialog);
        nameEdit->setPlaceholderText("e.g. coinmaster");
        nameEdit->setMaxLength(20);
        nameEdit->setStyleSheet("QLineEdit{padding:8px 10px;border:1px solid #426a9a;border-radius:8px;background:#142744;color:#f6f8ff;font:600 13px 'Segoe UI';}");
        QHBoxLayout *actionLayout = new QHBoxLayout();
        actionLayout->addStretch();
        QPushButton *cancelBtn = new QPushButton("Cancel", &nameDialog);
        QPushButton *okBtn = new QPushButton("Start", &nameDialog);
        cancelBtn->setStyleSheet("QPushButton{padding:6px 14px;border-radius:8px;background:#2b3f5e;color:#f6f8ff;font:600 12px 'Segoe UI';}");
        okBtn->setStyleSheet("QPushButton{padding:6px 14px;border-radius:8px;background:#d08b24;color:#ffffff;font:700 12px 'Segoe UI';}");
        actionLayout->addWidget(cancelBtn);
        actionLayout->addWidget(okBtn);
        nameLayout->addWidget(nameTitle);
        nameLayout->addWidget(nameSub);
        nameLayout->addWidget(nameEdit);
        nameLayout->addLayout(actionLayout);
        connect(cancelBtn, &QPushButton::clicked, &nameDialog, &QDialog::reject);
        connect(okBtn, &QPushButton::clicked, &nameDialog, [&nameDialog, nameEdit](){
            if(nameEdit->text().trimmed().isEmpty()){
                nameEdit->setStyleSheet("QLineEdit{padding:8px 10px;border:1px solid #d45f5f;border-radius:8px;background:#142744;color:#f6f8ff;font:600 13px 'Segoe UI';}");
                return;
            }
            nameDialog.accept();
        });
        nameEdit->setFocus();
        if(nameDialog.exec() != QDialog::Accepted){
            return;
        }
        const QString userName = nameEdit->text().trimmed();
        if(userName.isEmpty()){
            return;
        }
        GameSessionManager::beginSession(userName);
        startbtn->movedown();
        QTimer::singleShot(150, this, [=](){
        startbtn->setEnabled(false);
        startbtn->moveup();
        });
        QTimer::singleShot(300, this, [=](){
            startbtn->setEnabled(true);
            this->hide();
            this->mSelectScene.show();
            this->mSelectScene.move(this->pos());
        });
    });
}
