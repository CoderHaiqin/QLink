#include "map.h"

Map::Map()
{
    //注意顺序
    QObject::connect(this, &Map::blocksUpdated, this, &Map::updateBlockSet);
    QObject::connect(this, &Map::blocksUpdated, this, &Map::updateIsSolvable);
}

/**
 * @brief 默认情况下的初始化，设置连接线，提示持续时间
 * 生成地图
 * 更新blockSet
 */
void Map::init()
{
    lineTime[0] = 0.0f;
    lineTime[1] = 0.0f;
    itemGenerateCD = 0.0f;

    srand(time(NULL));

    generateMap();

    emit blocksUpdated();
}


void Map::init(const QString& mapInfo)
{
    srand(time(NULL));

    QStringList list = mapInfo.split(" ");
    QString blockArray = list[0];
    float itemCD = list[1].toFloat();

    int index = -1;
    for(int i = 0;i < GRID_SIZE_X;++i){
        for(int j = 0;j < GRID_SIZE_Y;++j){
            index++;
            if(blockArray[index] == '-'){
                grid[i][j] = -1;
                continue;
            }
            grid[i][j] = blockArray[index].toLatin1() - '0';
        }
    }
    itemGenerateCD = itemCD;

    emit blocksUpdated();
}

/**
 * @brief 生成地图
 * 首先初始化二维数组grid
 */
void Map::generateMap()
{
    for(int i = 0;i < GRID_SIZE_X;++i){
        for(int j = 0;j < GRID_SIZE_Y;++j){
            grid[i][j] = NON_BLOCK;
        }
    }

    int count = 0;
    for(int i = 0;i < GRID_SIZE_X / 2 * 2;i += 2){
        for(int j = 0;j < GRID_SIZE_Y / 5 + 1;++j){
            grid[i][j] = count % NUM_OF_BLOCK_IMAGES;
            grid[i + 1][j] = grid[i][j];
            count++;
        }
    }
}


QString Map::saveAsString()
{
    QString res = "";
    for(int i = 0;i < GRID_SIZE_X;++i){
        for(int j = 0;j < GRID_SIZE_Y;++j){

            if(grid[i][j] == NON_BLOCK){
                res.append('-');
            }
            else{
                res.append('0' + grid[i][j]);
            }

        }
    }

    res += " " + QString::number(itemGenerateCD);
    return res;
}

int* Map::operator[](const int i)
{
    return grid[i];
}
/**
  * @brief 检查连通性
  * @param 参数为两个方块的坐标
  * @return 能连通就返回true，反之返回false
*/
bool Map::isConnectable(const int x1, const int y1, const int x2, const int y2, const int playerIndex)
{
    if(grid[x1][y1] != grid[x2][y2] || grid[x1][y1] == NON_BLOCK || grid[x2][y2] == NON_BLOCK){
        return false;
    }

    if(y1 != y2 && checkVertically(x1, y1, x2, y2, playerIndex)){
        return true;
    }
    if(x1 != x2 && checkHorizontally(x1, y1, x2, y2, playerIndex)){
        return true;
    }

    return false;
}
/**
 * @brief 在垂直方向上检查
 */
bool Map::checkVertically(const int x1, const int y1, const int x2, const int y2, const int playerIndex)
{
    int range[2][2] = { {x1, x1}, {x2, x2} };

    while(range[0][0] > 0 && grid[range[0][0] - 1][y1] == -1) range[0][0]--;
    while(range[0][1] < GRID_SIZE_X - 1 && grid[range[0][1] + 1][y1] == -1) range[0][1]++;
    while(range[1][0] > 0 && grid[range[1][0] - 1][y2] == -1) range[1][0]--;
    while(range[1][1] < GRID_SIZE_X - 1 && grid[range[1][1] + 1][y2] == -1) range[1][1]++;

    int startRow = (range[0][0] > range[1][0]) ? range[0][0] : range[1][0];
    int endRow = (range[0][1] < range[1][1]) ? range[0][1] : range[1][1];
    int startColumn = (y1 < y2) ? y1 : y2;
    int endColumn = (y1 > y2) ? y1 : y2;

    int offset = ((x1 < x2) ? x1 : x2) - startRow;
    if(offset < 0) offset = 0;
    int length = endRow - startRow + 1;
    for(int i = 0;i < length;++i){
        int currentRow = startRow + offset % length;
        bool res = true;
        for(int j = startColumn + 1;j < endColumn;++j){
            if(grid[currentRow][j] != -1){
                res = false;
                break;
            }
        }
        if(res){
            if(playerIndex != -1){
                line[playerIndex].clear();
                line[playerIndex].append(Block(x1 * BLOCK_SIZE, y1 * BLOCK_SIZE));
                line[playerIndex].append(Block(currentRow * BLOCK_SIZE, y1 * BLOCK_SIZE));
                line[playerIndex].append(Block(currentRow * BLOCK_SIZE, y2 * BLOCK_SIZE));
                line[playerIndex].append(Block(x2 * BLOCK_SIZE, y2 * BLOCK_SIZE));
                lineTime[playerIndex] = LINE_TIME;
            }
            return true;
        }
        offset++;
    }
    return false;
}

/**
 * @brief 水平方向检查
 */
bool Map::checkHorizontally(const int x1, const int y1, const int x2, const int y2, const int playerIndex)
{
    int range[2][2] = { {y1, y1}, {y2, y2} };
    while(range[0][0] > 0 && grid[x1][range[0][0] - 1] == -1) range[0][0]--;
    while(range[0][1] < GRID_SIZE_Y - 1 && grid[x1][range[0][1] + 1] == -1) range[0][1]++;
    while(range[1][0] > 0 && grid[x2][range[1][0] - 1] == -1) range[1][0]--;
    while(range[1][1] < GRID_SIZE_Y - 1 && grid[x2][range[1][1] + 1] == -1) range[1][1]++;

    int startColumn = (range[0][0] > range[1][0]) ? range[0][0] : range[1][0];
    int endColumn = (range[0][1] < range[1][1]) ? range[0][1] : range[1][1];
    int startRow = (x1 < x2) ? x1 : x2;
    int endRow = (x1 > x2) ? x1 : x2;

    int offset = ((y1 < y2) ? y1 : y2) - startColumn;
    if(offset < 0) offset = 0;
    int length = endColumn - startColumn + 1;
    for(int i = 0;i < length;++i){
        int currentColumn = startColumn + offset % length;
        bool res = true;
        for(int j = startRow + 1;j < endRow;++j){
            if(grid[j][currentColumn] != -1){
                res = false;
                break;
            }
        }
        if(res){
            if(playerIndex != -1){
                line[playerIndex].clear();
                line[playerIndex].append(Block(x1 * BLOCK_SIZE, y1 * BLOCK_SIZE));
                line[playerIndex].append(Block(x1 * BLOCK_SIZE, currentColumn * BLOCK_SIZE));
                line[playerIndex].append(Block(x2 * BLOCK_SIZE, currentColumn * BLOCK_SIZE));
                line[playerIndex].append(Block(x2 * BLOCK_SIZE, y2 * BLOCK_SIZE));
                lineTime[playerIndex] = LINE_TIME;
            }
            return true;
        }
        offset++;
    }
    return false;
}

/**
 * @brief 返回此时的有解性
 */
bool Map::isSolvable()
{
    return solvable;
}

/**
 * @brief 更新存放方块的集合
*/
void Map::updateBlockSet()
{
    for(int i = 0;i < NUM_OF_BLOCK_IMAGES;++i){
        blockSet[i].clear();
    }
    for(int i = 0;i < GRID_SIZE_X;++i){
        for(int j = 0;j < GRID_SIZE_Y;++j){
            if(grid[i][j] >= 0 && grid[i][j] < NUM_OF_BLOCK_IMAGES){
                blockSet[grid[i][j]].append(Block(i, j));
            }
        }
    }
}

/**
 * @brief 检测有解性
 */
void Map::updateIsSolvable()
{
    //对于每一组方块
    for(int i = 0;i < NUM_OF_BLOCK_IMAGES;++i){
        int size = blockSet[i].size();
        //寻找有无可行解
        for(int j = 0;j < size;++j){
            for(int k = j + 1;k < size;++k){
                if(isConnectable(blockSet[i][j].x, blockSet[i][j].y, blockSet[i][k].x, blockSet[i][k].y, -1)){
                    //找到即返回，考虑到无解情况出现时，场上方块数量较少，所以实际运行耗时并不多
                    //第一次测试时，即使每次循环都调用该函数，也没有卡顿的情况
                    solvable = true;
                    return;
                }
            }
        }
    }
    solvable = false;
}

/**
 * @brief 方块重排
 * 这里需要保证重排后方块不会和player重合
 * 所以以当前玩家的位置作为参数
 */
void Map::shuffle(const QList<Block>& Positions)
{
    //用临时方块标记玩家的位置
    for(auto i : Positions){
        grid[i.x][i.y] = TEMP_BLOCK;

    }

    //因为地图用二维数组存储，可以直接用stl提供的random_shuffle
    std::random_shuffle(grid[0], &grid[GRID_SIZE_X][0]);

    //将临时方块与玩家所在位置的方块调换，并删去临时方块
    //从而避免打乱后方块的位置与玩家重合
    auto it = Positions.begin();
    for(int i = 0;i < GRID_SIZE_X;++i){
        for(int j = 0;j < GRID_SIZE_Y;++j){
            if(grid[i][j] == TEMP_BLOCK){
                grid[i][j] = grid[it->x][it->y];
                grid[it->x][it->y] = NON_BLOCK;
                it++;
            }
        }
    }

    //隐藏连接线
    lineTime[0] = 0.0f;
    lineTime[1] = 0.0f;
    //更新方块集合，hintPair等
    emit blocksUpdated(SHUFFLED);
}

/**
 * @brief 选取一个空白位置
 */
Block Map::getEmptyPosition()
{
    //随机选取一个位置
    int index = rand() % (GRID_SIZE_X * GRID_SIZE_Y);

    //该位置为空，直接返回
    if(grid[index / GRID_SIZE_Y][index % GRID_SIZE_Y] == NON_BLOCK){
        int x = index / GRID_SIZE_Y;
        int y = index % GRID_SIZE_Y;

        return Block(x, y);
    }
    //如果不为空，向后寻找
    int i = (index + 1) % (GRID_SIZE_X * GRID_SIZE_Y);
    while(i != index){
        if(grid[i / GRID_SIZE_Y][i % GRID_SIZE_Y] == NON_BLOCK){
            return Block(i / GRID_SIZE_Y, index % GRID_SIZE_Y);
        }
        i++;
        i %= GRID_SIZE_X * GRID_SIZE_Y;
    }
    //还是找不到，返回{-1, -1}
    return {-1, -1};
}

/**
 * @brief 连接一对方块
 */

bool Map::connect(const int x1, const int y1, const int x2, const int y2, const int playerIndex)
{
    if(isConnectable(x1, y1, x2, y2, playerIndex)){
        grid[x1][y1] = NON_BLOCK;
        grid[x2][y2] = NON_BLOCK;

        emit blocksUpdated(ERASERED);
        return true;
    }
    return false;
}

/**
  * @brief 每帧调用更新map的状态，主要是更新连接线显示的剩余时间和新道具生成的剩余时间
*/
void Map::update()
{
    for(int i = 0;i < 2;++i){
        if(lineTime[i] <= 0.0f){
            lineTime[i] = 0.0f;
            line[i].clear();
        }
        else{
            lineTime[i] -= 0.02f;
        }
    }

    if(itemGenerateCD <= 0.0f){
        generateItem();
    }else{
        itemGenerateCD -= 0.02f;
    }

}

void Map::paint(QPainter *painter, const Render& render)
{
    //绘制背景
    painter->setBrush(Qt::white);
    painter->drawRect(0, 0, WINDOW_HEIGHT, WINDOW_HEIGHT);

    //绘制方块
    for(int i = 0;i < GRID_SIZE_X;++i){
        for(int j = 0;j < GRID_SIZE_Y;++j){
            if(grid[i][j] != -1){
                render.paint(painter, i * BLOCK_SIZE, j * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, grid[i][j]);
            }
        }
    }

    //绘制连接线
    painter->save();
    for(int i = 0;i < 2;i++){
        if(!line[i].empty()){
            if(i == 0){
                painter->setPen(QPen(QBrush(Qt::red), 10));
            }else{
                painter->setPen(QPen(QBrush(Qt::blue), 10));
            }
            for(int j = 0;j < 3;++j){
                painter->drawLine(line[i][j].x + BLOCK_SIZE / 2, line[i][j].y + BLOCK_SIZE / 2, line[i][j + 1].x + BLOCK_SIZE / 2, line[i][j + 1].y + BLOCK_SIZE / 2);
            }
        }
    }
    painter->restore();
}

/**
 * @brief 生成道具
 */
void Map::generateItem()
{
    Block position = getEmptyPosition();
    while(position == Block(-1, -1) || grid[position.x][position.y] != NON_BLOCK){
        position = getEmptyPosition();
    }
    int type = rand() % 3;
    switch (type){
    case 0:
        grid[position.x][position.y] = BLOCK_TIME;
        break;
    case 1:
        grid[position.x][position.y] = BLOCK_HINT;
        break;
    default:
        grid[position.x][position.y] = BLOCK_SHUFFLE;

    }

    itemGenerateCD = ITEM_GENERATE_CD;
}


/**
 * @brief 获取一对可行的解，供hintPair使用
 * 注意要保证其随机性
 */
QVector<Block> Map::getHintPair()
{
    QVector<Block> hintPair(2);

    QVector<int> sequence(NUM_OF_BLOCK_IMAGES);
    for(int i = 0;i < NUM_OF_BLOCK_IMAGES;++i){
        sequence[i] = i;
    }
    //打乱各个blockSet的内部顺序，保证hint能随机地获取到可行的解
    for(int i = 0;i < NUM_OF_BLOCK_IMAGES;++i){
        std::random_shuffle(blockSet[i].begin(), blockSet[i].end());
    }
    std::random_shuffle(sequence.begin(), sequence.end());

    //对于每一组方块
    for(int i = 0;i < NUM_OF_BLOCK_IMAGES;++i){
        int size = blockSet[sequence[i]].size();
        //寻找有无可行解
        for(int j = 0;j < size;++j){
            for(int k = j + 1;k < size;++k){
                if(isConnectable(blockSet[sequence[i]][j].x, blockSet[sequence[i]][j].y, blockSet[sequence[i]][k].x, blockSet[sequence[i]][k].y, -1)){
                    hintPair[0] = { blockSet[sequence[i]][j].x, blockSet[sequence[i]][j].y };
                    hintPair[1] = { blockSet[sequence[i]][k].x, blockSet[sequence[i]][k].y };
                    return hintPair;
                }
            }
        }
    }
    return hintPair;
}
