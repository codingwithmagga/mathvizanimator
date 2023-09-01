// Needed for moc generation of Qt

#include "abstractobject.h"

AbstractObject::AbstractObject(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    connect(this, &QQuickItem::widthChanged, [=] { update(); });
    connect(this, &QQuickItem::heightChanged, [=] { update(); });
}

QString AbstractObject::name() const
{
    return name_;
}

void AbstractObject::setName(const QString &name)
{
    name_ = name;
}

QColor AbstractObject::color() const
{
    return color_;
}

void AbstractObject::setColor(const QColor &color)
{
    color_ = color;
}

qreal AbstractObject::mousearea_width() const
{
    return mousearea_width_;
}

void AbstractObject::setMousearea_width(const qreal newMousearea_width)
{
    mousearea_width_ = newMousearea_width;
}

qreal AbstractObject::mousearea_height() const
{
    return mousearea_height_;
}

void AbstractObject::setMousearea_height(qreal newMousearea_height)
{
    mousearea_height_ = newMousearea_height;
}
