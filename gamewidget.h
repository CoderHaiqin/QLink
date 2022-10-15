#pragma once
#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <random>
#include <QFileDialog>
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GameWidget; }
QT_END_NAMESPACE

//GameWidget类，只负责处理输入和切换窗口，具体的游戏逻辑放在Game类实现

class GameWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::GameWidget *ui;

    QTimer* timer;

    Game game;
    Player player;
public:
    GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

    void init(int gameMode);

public slots:
    void update();
    void gameover(const QString& gameoverMsg);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void backToTitle();
};
