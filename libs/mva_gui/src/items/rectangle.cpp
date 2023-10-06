#include "rectangle.h"

#include <QPainter>
#include <QPen>

RectangleItem::RectangleItem(QQuickItem *parent)
    : AbstractItem{"qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml", parent}
{}

void RectangleItem::paint(QPainter *painter)
{
    painter->save();

    const qreal pen_width = 4;

    QPen pen(color(), pen_width);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawRect(x() + pen_width / 2.0,
                      y() + pen_width / 2.0,
                      width() - pen_width,
                      height() - pen_width);
    painter->restore();
}
