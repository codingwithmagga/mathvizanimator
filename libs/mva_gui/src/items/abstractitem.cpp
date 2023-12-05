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

AbstractItem::AbstractItem(const QString& qml_file, QQuickItem* parent)
    : QQuickPaintedItem(parent), m_qml_file(qml_file) {}

QString AbstractItem::name() const { return m_name; }

void AbstractItem::setName(const QString& name) {
  if (m_name == name) {
    return;
  }

  m_name = name;
  emit nameChanged(m_name);
}

QColor AbstractItem::color() const { return m_color; }

void AbstractItem::setColor(const QColor& color) {
  if (m_color == color) {
    return;
  }
  m_color = color;
  emit colorChanged(m_color);
}

QJsonObject AbstractItem::toJson() const {
  QJsonObject json;
  auto properties = getItemProperties();
  auto parent_properties = getParentItemProperties();

  properties.append(QPair<QString, QVariant>{"file", m_qml_file});

  for (auto& property : properties) {
    json["item." + property.first] = property.second.toString();
  }

  for (auto& property : parent_properties) {
    json[property.first] = property.second.toString();
  }

  return json;
}

QList<QPair<QString, QVariant>> AbstractItem::getItemProperties() const {
  auto meta_object = metaObject();
  const auto properties = editableProperties();

  QList<QPair<QString, QVariant>> prop_list;

  do {
    prop_list.append(appendProperties(this, meta_object,
                                      properties.abstract_item_properties));
  } while ((meta_object = meta_object->superClass()));

  return prop_list;
}

QList<QPair<QString, QVariant>> AbstractItem::getParentItemProperties() const {
  auto parent_meta_object = parentItem()->metaObject();
  const auto properties = editableProperties();

  QList<QPair<QString, QVariant>> prop_list;

  do {
    prop_list.append(appendProperties(parentItem(), parent_meta_object,
                                      properties.quick_item_properties));
  } while ((parent_meta_object = parent_meta_object->superClass()));

  return prop_list;
}

void AbstractItem::paintItem(QPainter* painter) {
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

QList<QPair<QString, QVariant>> AbstractItem::appendProperties(
    const auto obj, auto meta_object,
    const QStringList& allowedProperties) const {
  QList<QPair<QString, QVariant>> prop_list;

  for (auto i = meta_object->propertyOffset(); i < meta_object->propertyCount();
       ++i) {
    if (allowedProperties.contains(QString(meta_object->property(i).name()))) {
      prop_list.emplace_back(meta_object->property(i).name(),
                             meta_object->property(i).read(obj));
    }
  }

  return prop_list;
}

AbstractItem::EditableProperties AbstractItem::editableProperties() const {
  EditableProperties editable_properties;

  editable_properties.abstract_item_properties =
      QStringList{"name", "color", "opacity", "rotation"};
  editable_properties.quick_item_properties =
      QStringList{"width", "height", "x", "y"};

  return editable_properties;
}
