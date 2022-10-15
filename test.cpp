#include "test.h"

Test::Test()
{
    for(int i = 0;i < GRID_SIZE_X;i++){
        for(int j = 0;j < GRID_SIZE_Y;j++){
            map.grid[i][j] = NON_BLOCK;
        }
    }
    map.grid[0][0] = BLOCK_RED;
    map.grid[2][0] = BLOCK_RED;

    map.grid[0][1] = BLOCK_BLUE;
    map.grid[2][2] = BLOCK_BLUE;

    map.grid[0][3] = BLOCK_GREEN;
    map.grid[2][3] = BLOCK_BLUE;
    map.grid[0][5] = BLOCK_BLUE;
    map.grid[2][5] = BLOCK_GREEN;

    map.grid[5][5] = BLOCK_ORANGE;
    map.grid[5][6] = BLOCK_RED;
    map.grid[6][6] = BLOCK_ORANGE;
    map.grid[6][5] = BLOCK_RED;
}

void Test::linkFail_color()
{
    bool result = map.connect(5, 5, 5, 6, -1);
    QVERIFY(result == false);
}

void Test::linkFail()
{
    bool result = map.connect(5, 5, 6, 6, -1);
    QVERIFY(result == false);
}

void Test::linkDirectly()
{
    bool result = map.connect(0, 0, 2, 0, -1);
    QVERIFY(result == true);
}

void Test::linkOneTurn()
{
    bool result = map.connect(0, 1, 2, 2, -1);
    QVERIFY(result == true);
}

void Test::linkTwoTurn()
{
    bool result = map.connect(0, 3, 2, 5, -1);
    QVERIFY(result == true);
}


Test::~Test()
{

}

#if TEST

QTEST_MAIN(Test)

#endif
