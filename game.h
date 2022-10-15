#pragma once

#include "player.h"
#include "map.h"
#include "render.h"
#include "hint.h"
#include <QMessageBox>
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

class Game : public QObject
{
    Q_OBJECT

    friend class Test;
private:
    QSet<int> inputKeys;

    float time;
    QFont font;
    int gameState;
    int gameMode;

    Player* player[2];
    Map map;
    Hint hinter;

    Render render;

    void checkGameOver();
    void inputProcess();
    void process();

    int collisionDetect(const int playerIndex);
    Block collidedBlock(Player* collisionPlayer, int collisionResult);
    void collisionHandler(const int playerIndex, const int collisionResult);
    void connect(const int playerIndex, const int x1, const int y1, const int x2, int y2);

    void shuffle();
    void hint();
    void timePlus();

    void drawInfo(QPainter* painter);

public:
    Game();
    ~Game();

    void init(const int gameMode);
    void init(const QString fileName);

    void paint(QPainter* painter);

    void update();

    void getInput(const int key);
    void releaseInput(const int key);

    void pause();
    void restart();

    void save(const QString fileName);
signals:
    void gameover(const QString& gameoverMsg);
};

