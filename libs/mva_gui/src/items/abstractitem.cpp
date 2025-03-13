/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "abstractitem.h"

#include <QPainter>

#include "basic_item.h"

AbstractItem::AbstractItem(const QString& qml_file, BasicItem* parent)
    : QQuickPaintedItem(parent)
    , m_qml_file(qml_file)
{
}

QString AbstractItem::name() const { return m_name; }

void AbstractItem::setName(const QString& name)
{
    if (m_name == name) {
        return;
    }

    m_name = name;
    emit nameChanged(m_name);
}

QJsonObject AbstractItem::toJson() const
{
    QJsonObject json;
    auto properties = itemProperties();
    auto parent_properties = parentItemProperties();

    properties.insert("file", m_qml_file);

    for (auto [property, value] : properties.asKeyValueRange()) {
        json["abstract_item." + property] = value.toString();
    }

    for (auto [property, value] : parent_properties.asKeyValueRange()) {
        json[property] = value.toString();
    }

    return json;
}

PropertyMap AbstractItem::allItemProperties() const
{
    auto propertyMap = itemProperties();
    propertyMap.insert(parentItemProperties());

    return propertyMap;
}

PropertyMap AbstractItem::itemProperties() const
{
    PropertyMap properties;
    auto meta_object = metaObject();

    do {
        properties.insert(fillPropertyMap(meta_object));
    } while ((meta_object = meta_object->superClass()));

    return properties;
}

PropertyMap AbstractItem::parentItemProperties() const
{
    PropertyMap properties;
    auto parent_meta_object = parentItem()->metaObject();

    do {
        properties.insert(fillPropertyMapParent(parent_meta_object));
    } while ((parent_meta_object = parent_meta_object->superClass()));

    return properties;
}

void AbstractItem::paintItem(QPainter* painter)
{
    painter->save();

    painter->translate(parentItem()->position());
    painter->setOpacity(opacity());

    if (rotation() != 0) {
        QPointF item_middle_point(width() / 2.0, height() / 2.0);
        painter->translate(item_middle_point);
        painter->rotate(rotation());
        painter->translate(-item_middle_point);
    }

    paint(painter);

    painter->restore();
}

PropertyMap AbstractItem::fillPropertyMap(const QMetaObject* const meta_object) const
{
    PropertyMap properties;

    for (auto i = meta_object->propertyOffset(); i < meta_object->propertyCount(); ++i) {
        if (editableProperties().contains(QString(meta_object->property(i).name()))) {
            properties.insert(meta_object->property(i).name(), meta_object->property(i).read(this));
        }
    }

    return properties;
}

PropertyMap AbstractItem::fillPropertyMapParent(const QMetaObject* const meta_object) const
{
    PropertyMap properties;

    for (auto i = meta_object->propertyOffset(); i < meta_object->propertyCount(); ++i) {
        if (editablePropertiesParent().contains(QString(meta_object->property(i).name()))) {
            properties.insert(meta_object->property(i).name(), meta_object->property(i).read(parentItem()));
        }
    }

    return properties;
}
