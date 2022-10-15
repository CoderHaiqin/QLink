#pragma once
#include <QPainter>
#include "constant.h"

//这个类提供了绘制贴图的工具

class Render
{
    QPixmap* texture;
public:
    Render();
    ~Render();

    void paint(QPainter* painter, const int x, const int y, const int sizeX, const int sizeY, const int textureIndex) const;
};
