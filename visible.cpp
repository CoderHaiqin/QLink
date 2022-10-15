#include "visible.h"

Visible::Visible(Texture* const imageSource) :
    texture(imageSource)
{}

QPainterPath Visible::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return path;
}

QRectF Visible::boundingRect() const
{
    return QRectF(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
