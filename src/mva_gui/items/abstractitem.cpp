// Needed for moc generation of Qt

#include "abstractitem.h"

AbstractItem::AbstractItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    connect(this, &QQuickItem::widthChanged, [=] { update(); });
    connect(this, &QQuickItem::heightChanged, [=] { update(); });
}

QString AbstractItem::name() const
{
    return name_;
}

void AbstractItem::setName(const QString &name)
{
    name_ = name;
}

QColor AbstractItem::color() const
{
    return color_;
}

void AbstractItem::setColor(const QColor &color)
{
    color_ = color;
}

qreal AbstractItem::mousearea_width() const
{
    return mousearea_width_;
}

void AbstractItem::setMousearea_width(const qreal newMousearea_width)
{
    mousearea_width_ = newMousearea_width;
}

qreal AbstractItem::mousearea_height() const
{
    return mousearea_height_;
}

void AbstractItem::setMousearea_height(qreal newMousearea_height)
{
    mousearea_height_ = newMousearea_height;
}
