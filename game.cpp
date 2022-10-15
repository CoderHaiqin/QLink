#include "game.h"

/**
  * @brief 创建game对象的初始化，主要是绘制部分的初始化
*/
Game::Game() :
    gameState(GAME_LOADING),
    hinter(map)
{
    font.setPixelSize(36);
    player[0] = nullptr;
    player[1] = nullptr;

    QObject::connect(&map, &Map::blocksUpdated, &hinter, &Hint::update);
}

/**
 * @brief 删除两个player对象，其他的没有用指针和new，所以不用管了
 */
Game::~Game()
{
    if(player[0]){
        delete player[0];
        player[0] = nullptr;
    }
    if(player[1]){
        delete player[1];
        player[1] = nullptr;
    }
}

/**
  * @brief init的一个重载，根据选择的游戏模式初始化游戏
  * @param 游戏模式
*/
void Game::init(const int gameMode)
{
    this->gameMode = gameMode;
    map.init();
    hinter.init();
    time = 5.0f;

    //创建player

    Block position = map.getEmptyPosition();
    while(position == Block(-1, -1) || map[position.x][position.y] != NON_BLOCK){
        position = map.getEmptyPosition();
    }
    player[0] = new Player;
    player[0]->init(0);
    player[0]->setPosition(position.x * BLOCK_SIZE, position.y * BLOCK_SIZE);

    if(gameMode == DOUBLE_MODE){
        Block newPosition;
        do {
            newPosition = map.getEmptyPosition();
        } while(position == Block(-1, -1) || newPosition == position || map[newPosition.x][newPosition.y] != NON_BLOCK);
        player[1] = new Player;
        player[1]->init(1);
        player[1]->setPosition(newPosition.x * BLOCK_SIZE, newPosition.y * BLOCK_SIZE);
    }

    shuffle();
    gameState = GAME_ONGOING;
}

/**
  * @brief init的一个重载，负责读取存档，逐行读取
  * @param 存档文件的位置
*/
void Game::init(const QString fileName)
{
    //打开存档文件
    QFile data(fileName);
    data.open(QFile::ReadOnly);
    QTextStream stream(&data);

    //读取游戏模式
    QString modeString = stream.readLine();
    gameMode = modeString[0].toLatin1() - '0';

    //读取时间
    QString timeString = stream.readLine();
    time = timeString.toDouble();

    //读取map，把这一行转交给map对象
    QString mapString = stream.readLine();
    map.init(mapString);

    //读取玩家信息
    QString playerInfo = stream.readLine();
    player[0] = new Player;
    player[0]->init(playerInfo, 0);

    if(gameMode == DOUBLE_MODE){
        QString playerInfo = stream.readLine();
        player[1] = new Player;
        player[1]->init(playerInfo, 1);
    }

    data.close();

    gameState = GAME_ONGOING;
}

/**
  * @brief 保存游戏，利用游戏状态生成多行字符串并存入文件
  * @param 存档文件的位置
*/

void Game::save(const QString fileName)
{
    QFile data(fileName);
    data.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    QTextStream file(&data);

    //保存游戏模式
    file << QString(gameMode + '0') << '\n';
    //保存时间
    file << QString::number(time) << '\n';
    //保存地图
    file << map.saveAsString() << '\n';
    //保存player
    file << player[0]->saveAsString() << '\n';
    if(gameMode == DOUBLE_MODE){
        file << player[1]->saveAsString() << '\n';
    }
}

/**
  * @brief 游戏主循环中调用的update函数，每帧调用一次
**/
void Game::update()
{
    if(gameState == GAME_ONGOING){
        inputProcess();
        process();
    }

    if(gameState == GAMEOVER_TIMEUP){
        gameState = GAME_PAUSE;
        QString gameoverMsg = "time up\n";
        if(gameMode == DOUBLE_MODE){

            if(player[0]->score > player[1]->score){
                gameoverMsg += "player1 is the winner";
            }else if(player[0]->score < player[1]->score){
                gameoverMsg += "player2 is the winner";
            }else{
                gameoverMsg += "no winner";
            }

        }
        emit gameover(gameoverMsg);
    }
    if(gameState == GAMEOVER_NOSOLUTION){
        gameState = GAME_PAUSE;
        QString gameoverMsg = "no solution\n";
        if(gameMode == DOUBLE_MODE){

            if(player[0]->score > player[1]->score){
                gameoverMsg += "player1 is the winner";
            }else if(player[0]->score < player[1]->score){
                gameoverMsg += "player2 is the winner";
            }else{
                gameoverMsg += "no winner";
            }

        }
        emit gameover(gameoverMsg);
    }
}

/**
  * @brief 获取输入，将按下的按键读取到缓冲区
  * @param Widget类通过调用keyPressEvent函数获取的输入
*/
void Game::getInput(const int key)
{
    switch (gameState){
    case GAME_PAUSE:
        if(key == Qt::Key_P) restart();
        break;
    case GAME_ONGOING:
        if(key == Qt::Key_P) pause();
        else inputKeys.insert(key);
    }
}

/**
  * @brief 同样是获取输入的函数，但这次获取的是松开的按键，将其从缓冲区移除
  * @param Widget类通过调用keyPressEvent函数获取的输入
*/
void Game::releaseInput(const int key)
{
    inputKeys.remove(key);
}

/**
  * @brief 处理输入，根据缓冲区所存储的输入调用Player类中的move函数
*/
void Game::inputProcess()
{
    bool noInput = true;
    if(inputKeys.contains(Qt::Key_W)){
        player[0]->move(UP);
        noInput = false;
    }
    else if(inputKeys.contains(Qt::Key_S)){
        player[0]->move(DOWN);
        noInput = false;
    }
    if(inputKeys.contains(Qt::Key_A)){
        player[0]->move(LEFT);
        noInput = false;
    }
    else if(inputKeys.contains(Qt::Key_D)){
        player[0]->move(RIGHT);
        noInput = false;
    }
    //如果缓冲区内没有相应的按键，就调用stop函数
    if(noInput) player[0]->stop();

    if(gameMode == DOUBLE_MODE){
        noInput = true;
        if(inputKeys.contains(Qt::Key_Up)){
            player[1]->move(UP);
            noInput = false;
        }
        else if(inputKeys.contains(Qt::Key_Down)){
            player[1]->move(DOWN);
            noInput = false;
        }
        if(inputKeys.contains(Qt::Key_Left)){
            player[1]->move(LEFT);
            noInput = false;
        }
        else if(inputKeys.contains(Qt::Key_Right)){
            player[1]->move(RIGHT);
            noInput = false;
        }
        if(noInput) player[1]->stop();
    }
}

//设置gameState，暂停游戏
void Game::pause()
{
    gameState = GAME_PAUSE;
}

//设置gameState，继续进行游戏
void Game::restart()
{
    gameState = GAME_ONGOING;
}


/**
  * @brief 游戏正常进行时循环调用的过程
  * 主要完成以下几个任务：
  * 更新时间
  * 游戏结束条件的判定
  * 更新player，碰撞检测
  * 更新map
*/
void Game::process()
{
    time -= (float)PERIOD / 1000.0f;

    //检测是否gameover
    if(time <= 0){
        pause();
        gameState = GAMEOVER_TIMEUP;
    }
    if(!map.isSolvable()){
        pause();
        gameState = GAMEOVER_NOSOLUTION;
    }
    //对一号player进行碰撞检测，并更新其状态
    collisionHandler(0, collisionDetect(0));
    player[0]->update();

    //对二号player进行碰撞检测，并更新其状态
    if(gameMode == DOUBLE_MODE){
        collisionHandler(1, collisionDetect(1));
        player[1]->update();
    }

    //更新map的状态
    map.update();
    hinter.process();
}

/**
  * @brief 碰撞检测
  * @param player的序号
  * @return 发生碰撞的方向，参考constant.h
*/
int Game::collisionDetect(const int playerIndex)
{
    int res = 0;

    int radius = PLAYER_SIZE / 2;
    int centerX = player[playerIndex]->x + radius;
    int centerY = player[playerIndex]->y + radius;
    int gridX = centerX / BLOCK_SIZE;
    int gridY = centerY / BLOCK_SIZE;

    if(gridY > 0 && map[gridX][gridY - 1] != -1
        && centerY - radius < (gridY) * BLOCK_SIZE){
        res = UP;
    }
    else if(gridY < GRID_SIZE_Y - 1 && map[gridX][gridY + 1] != -1
        && centerY + radius > (gridY + 1) * BLOCK_SIZE){
        res = DOWN;
    }

    if(gridX > 0 && map[gridX - 1][gridY] != -1
        && centerX - radius < (gridX) * BLOCK_SIZE){
        res = LEFT;
    }
    else if(gridX < GRID_SIZE_X - 1 && map[gridX + 1][gridY] != -1
        && centerX + radius > (gridX + 1) * BLOCK_SIZE){
        res = RIGHT;
    }

    if(res == 0){
        if(gridX > 0 && gridY > 0 && map[gridX - 1][gridY - 1] != -1
            && pow(centerX % BLOCK_SIZE, 2) + pow(centerY % BLOCK_SIZE, 2) < pow(radius, 2)){
            res = LEFT_UP;
        }
        else if(gridX > 0 && gridY < GRID_SIZE_Y - 1 && map[gridX - 1][gridY + 1] != -1
            && pow(centerX % BLOCK_SIZE, 2) + pow(BLOCK_SIZE - centerY % BLOCK_SIZE, 2) < pow(radius, 2)){
            res = LEFT_DOWN;
        }
        else if(gridX < GRID_SIZE_X - 1 && gridY < GRID_SIZE_Y - 1 && map[gridX + 1][gridY + 1] != -1
            && pow(BLOCK_SIZE - centerX % BLOCK_SIZE, 2) + pow(BLOCK_SIZE - centerY % BLOCK_SIZE, 2) < pow(radius, 2)){
            res = RIGHT_DOWN;
        }
        else if(gridX < GRID_SIZE_X - 1 && gridY > 0 && map[gridX + 1][gridY - 1] != -1
            && pow(BLOCK_SIZE - centerX % BLOCK_SIZE, 2) + pow(centerY % BLOCK_SIZE, 2) < pow(radius, 2)){
            res = RIGHT_UP;
        }
    }
    return res;
}

Block Game::collidedBlock(Player* collisionPlayer, int collisionResult)
{

    int gridX = collisionPlayer->getGridX();
    int gridY = collisionPlayer->getGridY();

    //寻找发生碰撞的格子的坐标，一共八个方向，所以有点长
    if((collisionResult == UP) && collisionPlayer->direction == UP) { gridY--; collisionPlayer->stop(); }
    else if((collisionResult == DOWN) && collisionPlayer->direction == DOWN){ gridY++; collisionPlayer->stop(); }
    else if((collisionResult == LEFT) && collisionPlayer->direction == LEFT) { gridX--; collisionPlayer->stop(); }
    else if((collisionResult == RIGHT) && collisionPlayer->direction == RIGHT) { gridX++; collisionPlayer->stop(); }

    if(collisionResult == LEFT_UP && (collisionPlayer->direction == LEFT || collisionPlayer->direction == UP)){
        gridX--;
        gridY--;
        collisionPlayer->stop();
    }
    if(collisionResult == LEFT_DOWN && (collisionPlayer->direction == LEFT || collisionPlayer->direction == DOWN)){
        gridX--;
        gridY++;
        collisionPlayer->stop();
    }
    if(collisionResult == RIGHT_DOWN && (collisionPlayer->direction == RIGHT || collisionPlayer->direction == DOWN)){
        gridX++;
        gridY++;
        collisionPlayer->stop();
    }
    if(collisionResult == RIGHT_UP && (collisionPlayer->direction == RIGHT || collisionPlayer->direction == UP)){
        gridX++;
        gridY--;
        collisionPlayer->stop();
    }

    return Block(gridX, gridY);
}

/**
  * @brief 处理碰撞结果
  * @param player序号，发生碰撞的方向
*/
void Game::collisionHandler(const int playerIndex, const int collisionResult)
{
    Player* collisionPlayer = player[playerIndex];
    if(collisionResult == 0) return;

    Block targetBlock = collidedBlock(collisionPlayer, collisionResult);
    int gridX = targetBlock.x;
    int gridY = targetBlock.y;

    //如果与边缘发生碰撞直接退出
    if(gridX < 0 || gridX >= GRID_SIZE_X || gridY < 0 || gridY >= GRID_SIZE_Y) return;

    switch (map[gridX][gridY]){
    //发生碰撞位置是方块
    case BLOCK_RED:
    case BLOCK_BLUE:
    case BLOCK_ORANGE:
    case BLOCK_GREEN:
        //如果之前player已经选中方块，则尝试连接它们
        if(collisionPlayer->selectedBlock[0] != -1 &&
            (collisionPlayer->selectedBlock[0] != gridX || collisionPlayer->selectedBlock[1] != gridY)){
            connect(playerIndex, collisionPlayer->selectedBlock[0], collisionPlayer->selectedBlock[1], gridX, gridY);
        }
        //如果之前没有选中方块，则选择此时碰撞的方块
        else{
            collisionPlayer->selectedBlock[0] = gridX;
            collisionPlayer->selectedBlock[1] = gridY;
        }
        break;
    //碰到了shuffle道具
    case BLOCK_SHUFFLE:
        map[gridX][gridY] = NON_BLOCK;
        shuffle();
        break;
    //碰到了time道具
    case BLOCK_TIME:
        map[gridX][gridY] = NON_BLOCK;
        timePlus();
        break;
    //碰到了time道具
    case BLOCK_HINT:
        map[gridX][gridY] = NON_BLOCK;
        hint();
        break;
    }

}

/**
 * @brief 连接两个方块
 * 把方块坐标发送给map，根据结果设置player的选定状态
 */
void Game::connect(const int playerIndex, const int x1, const int y1, const int x2, const int y2)
{
    //如果连接成功，取消player的选择位置
    //地图状态的更新 被委派给map类
    if(map.connect(x1, y1, x2, y2, playerIndex)){
        player[playerIndex]->cancelSelection();
        player[playerIndex]->getScore();
    }
    //如果连接失败，player转而选择本次碰撞的方块
    else{
        player[playerIndex]->select(x2, y2);
    }
}


/**
  * @brief 绘制游戏画面
  * @param painter
*/
void Game::paint(QPainter *painter)
{
    painter->save();
    //绘制地图
    map.paint(painter, render);
    hinter.paint(painter, render);
    //绘制player
    player[0]->paint(painter, render);
    if(gameMode == DOUBLE_MODE){
        player[1]->paint(painter, render);
    }
    //绘制分数、时间等信息
    drawInfo(painter);

    painter->restore();
}

/**
  * @brief 显示分数、时间文字信息
  * @param 传入painter
*/
void Game::drawInfo(QPainter *painter)
{
    painter->setFont(font);
    painter->drawText(700, 50, "Time:");
    painter->drawText(700, 100, QString::number(time));

    if(gameMode == DOUBLE_MODE){
        painter->drawText(700, 150, "Player1 Score:");
        painter->drawText(700, 200, QString::number(player[0]->score));
        painter->drawText(700, 250, "Player2 Score:");
        painter->drawText(700, 300, QString::number(player[1]->score));
    }else{
        painter->drawText(700, 150, "Score:");
        painter->drawText(700, 200, QString::number(player[0]->score));
    }

}

/**
 * @brief 负责重排的函数，需要注意的是这里涉及到一些问题：
 * 必须保证重排后方块不会与玩家的位置重合，所以这里要把player的位置作为参数传入
 * 重排后还要取消选中
 */
void Game::shuffle()
{
    QList<Block> Positions;
    Positions.append(Block(player[0]->getGridX(), player[0]->getGridY()));
    if(gameMode == DOUBLE_MODE){
        Positions.append(Block(player[1]->getGridX(), player[1]->getGridY()));
    }
    map.shuffle(Positions);

    player[0]->cancelSelection();
    if(gameMode == DOUBLE_MODE){
        player[1]->cancelSelection();
    }
}

/**
 * @brief time道具
 */
void Game::timePlus()
{
    time += 10.0f;
}

/**
 * @brief hint道具
 */
void Game::hint()
{
    hinter.enable();
}
