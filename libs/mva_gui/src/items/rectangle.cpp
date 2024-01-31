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
    : AbstractItem { "qrc:/qt/qml/cwa/mva/gui/qml/items/MVARectangle.qml", parent }
{
}

void RectangleItem::paint(QPainter* painter)
{
    painter->save();

    const qreal pen_width = 4;

    QPen pen(color(), pen_width);
    pen.setJoinStyle(Qt::MiterJoin);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawRect(x() + pen_width / 2.0, y() + pen_width / 2.0, width() - pen_width, height() - pen_width);
    painter->restore();
}

AbstractItem::EditableProperties RectangleItem::editableProperties() const
{
    return AbstractItem::editableProperties();
}
