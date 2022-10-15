#pragma once
#include <QObject>
#include <QtTest>
#include "map.h"
#include "game.h"

class Test : public QObject
{
    Q_OBJECT
private:
    Map map;
public:
    Test();
    ~Test();
private slots:
    void linkFail_color();
    void linkFail();
    void linkDirectly();
    void linkOneTurn();
    void linkTwoTurn();
};
