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

  json["x"] = parentItem()->x();
  json["y"] = parentItem()->y();
  json["width"] = parentItem()->width();
  json["height"] = parentItem()->height();
  json["item.color"] = m_color.name();
  json["item.name"] = m_name;
  json["item.file"] = m_qml_file;

  return json;
}

void AbstractItem::setPropertiesFromJson(const QJsonObject& json) {
  if (const QJsonValue v = json["x"]; v.isDouble()) {
    parentItem()->setX(v.toDouble());
  }

  if (const QJsonValue v = json["y"]; v.isDouble()) {
    parentItem()->setY(v.toDouble());
  }

  if (const QJsonValue v = json["width"]; v.isDouble()) {
    parentItem()->setWidth(v.toDouble());
  }

  if (const QJsonValue v = json["height"]; v.isDouble()) {
    parentItem()->setHeight(v.toDouble());
  }

  if (const QJsonValue v = json["color"]; v.isString()) {
    m_color = v.toString();
  }

  if (const QJsonValue v = json["name"]; v.isString()) {
    m_name = v.toString();
  }
}

AbstractItem::EditableProperties AbstractItem::editableProperties() const {
  EditableProperties editableProperties;

  editableProperties.abstract_item_properties =
      QStringList{"name", "color", "opacity", "rotation"};
  editableProperties.quick_item_properties =
      QStringList{"width", "height", "x", "y"};

  return editableProperties;
}
