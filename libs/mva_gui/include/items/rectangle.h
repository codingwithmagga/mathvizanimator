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

#ifndef LIBS_MVA_GUI_INCLUDE_ITEMS_RECTANGLE_H_
#define LIBS_MVA_GUI_INCLUDE_ITEMS_RECTANGLE_H_

#include "abstractitem.h"

class RectangleItem : public AbstractItem {
    Q_OBJECT
    QML_ELEMENT

public:
    explicit RectangleItem(QQuickItem* parent = nullptr);

    void paint(QPainter* painter) override;

    EditableProperties editableProperties() const override;
};

#endif // LIBS_MVA_GUI_INCLUDE_ITEMS_RECTANGLE_H_
