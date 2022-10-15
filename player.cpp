#include "player.h"

Player::Player(){}

Player::~Player(){}

/**
 * @brief 初始化
 */
void Player::init(int index)
{
    x = 500;
    y = 500;
    score = 0;
    this->index = index;
    cancelSelection();
}

/**
 * @brief 从读取存档中初始化
 */
void Player::init(QString playerString, int index)
{
    QStringList playerInfo = playerString.split(" ");
    setPosition(playerInfo[0].toInt(), playerInfo[1].toInt());
    select(playerInfo[2].toInt(), playerInfo[3].toInt());
    score = playerInfo[4].toInt();

    this->index = index;
}

/**
 * @brief 把当前的状态输出为字符串
 */
QString Player::saveAsString()
{
    QString result = "";
    result += QString::number(x) + " " + QString::number(y) + " ";
    result += QString::number(selectedBlock[0]) + " " + QString::number(selectedBlock[1]) + " ";
    result += QString::number(score);
    return result;
}

int Player::getGridX() const
{
    return (x + PLAYER_SIZE / 2) / BLOCK_SIZE;
}

int Player::getGridY() const
{
    return (y + PLAYER_SIZE / 2) / BLOCK_SIZE;
}

/**
 * @brief 设置移动方向
 * 位置的改变并不是在此时进行的
 */
void Player::move(const int direction)
{
    this->direction = direction;
}

void Player::stop()
{
    direction = STOP;
}

/**
 * @brief 取消当前的选中
 */
void Player::cancelSelection()
{
    selectedBlock[0] = -1;
    selectedBlock[1] = -1;
}

void Player::setPosition(const int x, const int y)
{
    this->x = x;
    this->y = y;
}

void Player::select(const int x, const int y)
{
    this->selectedBlock[0] = x;
    this->selectedBlock[1] = y;
}

void Player::getScore()
{
    score += BLOCK_SCORE;
}

/**
 * @brief update，更新自身的位置
 */
void Player::update()
{
    switch (direction){
    case UP:
        if(y > 0) y -= SPEED;
        break;
    case DOWN:
        if(y < GRID_SIZE_Y * BLOCK_SIZE - PLAYER_SIZE) y += SPEED;
        break;
    case LEFT:
        if(x > 0) x -= SPEED;
        break;
    case RIGHT:
        if(x < GRID_SIZE_X * BLOCK_SIZE - PLAYER_SIZE) x += SPEED;
        break;
    default:
        break;
    }
}

void Player::paint(QPainter* painter, const Render &render)
{
    if(index == 0){
        render.paint(painter, x, y, PLAYER_SIZE, PLAYER_SIZE, PLAYER_1);
        render.paint(painter, selectedBlock[0] * BLOCK_SIZE, selectedBlock[1] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SELECTION_1);
    }else{
        render.paint(painter, x, y, PLAYER_SIZE, PLAYER_SIZE, PLAYER_2);
        render.paint(painter, selectedBlock[0] * BLOCK_SIZE, selectedBlock[1] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SELECTION_2);
    }
}
