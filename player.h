#pragma once

#include "constant.h"
#include "render.h"
#include <QString>
#include <QStringList>

class Player
{
public:
    int x, y;
    int direction;
    int score;
    int index;

    int selectedBlock[2];

    Player();
    ~Player();

    void init(int index);
    void init(QString playerString, int index);

    QString saveAsString();

    int getGridX() const;
    int getGridY() const;

    void setPosition(const int x, const int y);

    void select(const int x, const int y);
    void cancelSelection();
    void getScore();

    void move(const int newDirection);
    void stop();

    void update();
    void paint(QPainter* painter, const Render& render);
};
