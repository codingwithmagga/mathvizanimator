#include "rectangle.h"

#include <QPainter>
#include <QPen>

RectangleItem::RectangleItem(QQuickItem *parent)
    : AbstractObject{parent}
{}

AbstractObject::ObjectContour RectangleItem::getObjectContour() const
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

    qDebug() << "Paint rect: " << x() << y() << width() << height();
}

void RectangleItem::onPressed(const qreal &mouse_x, const qreal &mouse_y)
{
    start_x_ = mouse_x;
    start_y_ = mouse_y;
}

void RectangleItem::onPosChanged(const qreal &mouse_x, const qreal &mouse_y)
{
    // Prevent boundary violation
    qreal mouse_x_bdy = std::min(mousearea_width(), std::max(0.0, mouse_x));
    qreal mouse_y_bdy = std::min(mousearea_height(), std::max(0.0, mouse_y));

    if (mouse_x_bdy - start_x_ < 0) {
        parentItem()->setX(mouse_x_bdy);
    }

    if (mouse_y_bdy - start_y_ < 0) {
        parentItem()->setY(mouse_y_bdy);
    }

    setWidth(std::abs(mouse_x_bdy - start_x_));
    setHeight(std::abs(mouse_y_bdy - start_y_));
}

void RectangleItem::componentComplete()
{
    QQuickPaintedItem::componentComplete();
}
