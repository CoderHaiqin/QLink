#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include "constant.h"
#include "render.h"


class Visible : public QGraphicsItem
{
protected:

    const Texture* texture;

    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
public:
    Visible(Texture* const imageSource);
};

