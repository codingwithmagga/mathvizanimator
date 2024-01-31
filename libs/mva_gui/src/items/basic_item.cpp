/* mathvizanimator
 * Copyright (C) 2023-2024 codingwithmagga
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

#include "basic_item.h"

#include "abstractitem.h"

BasicItem::BasicItem(QQuickItem* parent)
    : QQuickItem(parent)
{
    setHeight(100);
    setWidth(100);
}

void BasicItem::setAbstractItem(AbstractItem* const new_item)
{
    if (new_item == m_abstract_item) {
        return;
    }
    m_abstract_item = new_item;
    emit abstractItemChanged();
}

QString BasicItem::abstractItemName() const { return m_abstract_item->name(); }
