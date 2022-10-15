#pragma once

#include <QObject>
#include "map.h"

class Hint : public QObject
{
    Q_OBJECT
private:
    Map& map;
    QVector<Block> hintPair;
    float hintTime;

    bool checkPair();
public:
    Hint(Map& map);
    virtual ~Hint() {}

    void enable();
    void process();
    void paint(QPainter* painter, const Render& render);
    void init();
public slots:
    void update(int signalType);
};
