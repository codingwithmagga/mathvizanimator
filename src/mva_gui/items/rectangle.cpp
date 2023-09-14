#include "rectangle.h"

#include <QPainter>
#include <QPen>

RectangleItem::RectangleItem(QQuickItem *parent)
    : AbstractItem{parent}
{}

AbstractItem::ObjectContour RectangleItem::getObjectContour() const
{
    const auto x = parentItem()->x();
    const auto y = parentItem()->y();

    Point bottom_left{x, y + height()};
    Point bottom_right{x + width(), y + height()};
    Point top_right{x + width(), y};
    Point top_left{x, y};

    BezierCurve line_bottom{bottom_left, bottom_left, bottom_right, bottom_right};
    BezierCurve line_right{bottom_right, bottom_right, top_right, top_right};
    BezierCurve line_top{top_right, top_right, top_left, top_left};
    BezierCurve line_left{top_left, top_left, bottom_left, bottom_left};

    ObjectContour object_contour;
    object_contour.contour = QVector<BezierCurve>{line_bottom, line_right, line_top, line_left};

    return object_contour;
}

void RectangleItem::paint(QPainter *painter)
{
    QPen pen(color(), 5);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawRect(x(), y(), width(), height());
}
