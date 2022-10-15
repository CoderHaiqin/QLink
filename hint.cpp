#include "hint.h"

/**
 * @brief 初始化
 */
Hint::Hint(Map& map) : map(map)
{
    hintTime = 0.0f;
    hintPair.resize(2);
}

void Hint::init()
{
    hintTime = 0.0f;
    hintPair.clear();
}

/**
 * @brief 启用hint
 */
void Hint::enable()
{
    hintTime = HINT_TIME;
}

/**
 * @brief 每次循环时调用
 */
void Hint::process()
{
    hintTime -= 0.02f;
}

/**
 * @brief 判断此时内部存储的hintPair是否有效
 */
bool Hint::checkPair()
{
    if(hintPair.empty()) return false;
    return map.isConnectable(hintPair[0].x, hintPair[0].y, hintPair[1].x, hintPair[1].y);
}

/**
 * @brief 如果此时内部存储的hintPair不再有效就寻找另一对
 */
void Hint::update(int signalType)
{
    if(signalType == SHUFFLED || !checkPair()){
        hintPair = map.getHintPair();
    }
}

/**
 * @brief 在被提示的方块上绘制标记
 */
void Hint::paint(QPainter *painter, const Render &render)
{
    if(hintTime <= 0.0f) return;

    painter->save();
    for(int i = 0;i < 2;i++){
        render.paint(painter, hintPair[i].x * BLOCK_SIZE, hintPair[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SELECTION_1);
    }
    painter->restore();
}
