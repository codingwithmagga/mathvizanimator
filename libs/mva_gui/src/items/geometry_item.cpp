#include "geometry_item.h"

GeometryItem::GeometryItem(const QString& qml_file, BasicItem* parent)
    : AbstractItem(qml_file, parent)
{
    connect(this, &QQuickItem::opacityChanged, this, [this]() { update(); });
}

QColor GeometryItem::filledColor() const { return m_filled_color; }

void GeometryItem::setFilledColor(const QColor& new_filled_color)
{
    if (m_filled_color == new_filled_color) {
        return;
    }
    m_filled_color = new_filled_color;
    emit filledColorChanged();
}

qreal GeometryItem::filledOpacity() const { return m_filled_opacity; }

void GeometryItem::setFilledOpacity(const qreal& new_filled_opacity)
{
    if (qFuzzyCompare(m_filled_opacity, new_filled_opacity)) {
        return;
    }
    m_filled_opacity = new_filled_opacity;
    emit filledOpacityChanged();
}

QColor GeometryItem::borderColor() const { return m_border_color; }

void GeometryItem::setBorderColor(const QColor& new_border_color)
{
    if (m_border_color == new_border_color) {
        return;
    }
    m_border_color = new_border_color;
    emit borderColorChanged();
}

qreal GeometryItem::borderOpacity() const { return m_border_opacity; }

void GeometryItem::setBorderOpacity(const qreal& new_border_opacity)
{
    if (qFuzzyCompare(m_border_opacity, new_border_opacity)) {
        return;
    }
    m_border_opacity = new_border_opacity;
    emit borderOpacityChanged();
}

qreal GeometryItem::borderWidth() const { return m_border_width; }

void GeometryItem::setBorderWidth(const qreal& new_border_width)
{
    if (qFuzzyCompare(m_border_width, new_border_width)) {
        return;
    }
    m_border_width = new_border_width;
    emit borderWidthChanged();
}

AbstractItem::EditableProperties GeometryItem::editableProperties() const
{
    auto abstractList = AbstractItem::editableProperties();

    abstractList.abstract_item_properties.append("filledColor");
    abstractList.abstract_item_properties.append("filledOpacity");

    abstractList.abstract_item_properties.append("borderColor");
    abstractList.abstract_item_properties.append("borderOpacity");
    abstractList.abstract_item_properties.append("borderWidth");

    return abstractList;
}
