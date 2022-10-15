#pragma once
#include "constant.h"
#include "render.h"
#include <QObject>
#include <QVector>
#include <QTime>
#include <random>
#include <QPainter>

struct Block
{
    int x;
    int y;

    Block(int x = -1, int y = -1) : x(x), y(y) {}
    Block(const Block& block) : x(block.x), y(block.y) {}
    bool operator ==(const Block block) {return x == block.x && y == block.y;}
};

class Map : public QObject
{
    Q_OBJECT

    friend class Test;
private:
    int grid[GRID_SIZE_X][GRID_SIZE_Y];

    QList<Block> blockSet[NUM_OF_BLOCK_IMAGES];
    bool solvable;

    QList<Block> line[2];
    float lineTime[2];

    float itemGenerateCD;
    void generateItem();
    void generateMap();

    bool checkVertically(const int x1, const int y1, const int x2, const int y2, const int playerIndex = -1);
    bool checkHorizontally(const int x1, const int y1, const int x2, const int y2, const int playerIndex = -1);
public:
    Map();

    int* operator [](const int i);

    Block getEmptyPosition();

    void init();
    void init(const QString& filename);

    QString saveAsString();

    bool isConnectable(const int x1, const int y1, const int x2, const int y2, const int playerIndex = -1);

    bool isSolvable();

    bool connect(const int x1, const int y1, const int x2, const int y2, const int playerIndex);

    void shuffle(const QList<Block>& Positions);

    QVector<Block> getHintPair();

    void update();

    void paint(QPainter* painter, const Render& render);
public slots:
    void updateBlockSet();
    void updateIsSolvable();
signals:
    void blocksUpdated(int signalType = -1);
};
