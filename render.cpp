#include "render.h"

/**
 * @brief 初始化，导入了所有图片资源的路径
 */
Render::Render()
{
    const QString imageName[NUM_OF_IMAGES] = {
        ":/images/red.png",
        ":/images/blue.png",
        ":/images/orange.png",
        ":/images/green.png",
        ":/images/shuffle.png",
        ":/images/time.png",
        ":/images/hint.png",
        ":/images/player.png",
        ":/images/selection1.png",
        ":/images/player2.png",
        ":/images/selection2.png"
    };

    texture = new QPixmap[NUM_OF_IMAGES];
    for(int i = 0;i < NUM_OF_IMAGES;++i){
        texture[i].load(imageName[i]);
    }
}

Render::~Render()
{
    delete texture;
}

/**
 * @brief 在相应的位置绘制图片资源
 */
void Render::paint(QPainter* painter, const int x, const int y, const int sizeX, const int sizeY, const int textureIndex) const
{
    painter->drawPixmap(x, y, sizeX, sizeY, texture[textureIndex]);
}
