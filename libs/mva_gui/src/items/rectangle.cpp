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

#include "rectangle.h"

#include <QPainter>
#include <QPen>

RectangleItem::RectangleItem(BasicItem* parent)
    : GeometryItem { "qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml", parent }
{
}

void RectangleItem::paint(QPainter* painter)
{
    painter->save();

    const qreal border_width = borderWidth();
    painter->setRenderHints(QPainter::Antialiasing, true);

    QPen pen_filled(QColor(), 0);
    pen_filled.setBrush(QBrush());
    painter->setPen(pen_filled);
    painter->setBrush(filledColor());
    painter->setOpacity(opacity() * filledOpacity());
    painter->drawRect(x() + border_width, y() + border_width, width() - 2 * border_width, height() - 2 * border_width);

    QPen pen_border(borderColor(), border_width);
    pen_border.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen_border);
    painter->setBrush(QBrush());
    painter->setOpacity(opacity() * borderOpacity());
    painter->drawRect(
        x() + border_width / 2.0, y() + border_width / 2.0, width() - border_width, height() - border_width);

    painter->restore();
}
