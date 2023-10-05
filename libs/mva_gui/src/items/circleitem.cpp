#include "circleitem.h"

#include <QPainter>
#include <QPen>

CircleItem::CircleItem(QQuickItem *parent)
    : AbstractItem{"qrc:/qt/qml/cwa/mva/gui/qml/items/MVACircle.qml", parent}
{}

void CircleItem::paint(QPainter *painter)
{
    painter->save();
    const auto pen_width = 4.0;

    QPen pen(color(), pen_width);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawEllipse(x() + pen_width / 2.0,
                         y() + pen_width / 2.0,
                         width() - pen_width,
                         height() - pen_width);
    painter->restore();
}
