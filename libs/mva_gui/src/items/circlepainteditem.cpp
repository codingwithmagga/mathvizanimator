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

#include "circlepainteditem.h"

#include <QPainter>
#include <QPen>
#include <QSGSimpleRectNode>

CirclePaintedItem::CirclePaintedItem(QQuickItem *parent)
    : AbstractItem{"qrc:/qt/qml/cwa/mva/gui/qml/items/MVAPaintedCircle.qml",
                   parent} {
  setFlag(ItemHasContents, true);
  setAntialiasing(true);
}

void CirclePaintedItem::paint(QPainter *painter) {
  painter->save();
  const auto pen_width = 4.0;

  QPen pen(color(), pen_width);
  painter->setPen(pen);
  painter->setRenderHints(QPainter::Antialiasing, true);
  painter->drawEllipse(x() + pen_width / 2.0, y() + pen_width / 2.0,
                       width() - pen_width, height() - pen_width);
  painter->restore();
}

AbstractItem::EditableProperties CirclePaintedItem::editableProperties() const {
  return AbstractItem::editableProperties();
}
