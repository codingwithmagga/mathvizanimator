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

#ifndef LIBS_MVA_GUI_INCLUDE_ITEMS_GEOMETRY_ITEM_H_
#define LIBS_MVA_GUI_INCLUDE_ITEMS_GEOMETRY_ITEM_H_

#include <QObject>

#include "abstractitem.h"

class GeometryItem : public AbstractItem {
    Q_OBJECT

    // There is already a filledColor property in QQuickPaintedItem so I call this "filledColor"
    Q_PROPERTY(QColor filledColor READ filledColor WRITE setFilledColor NOTIFY filledColorChanged)
    Q_PROPERTY(qreal filledOpacity READ filledOpacity WRITE setFilledOpacity NOTIFY filledOpacityChanged)

    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(qreal borderOpacity READ borderOpacity WRITE setBorderOpacity NOTIFY borderOpacityChanged)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

  public:
    explicit GeometryItem(const QString& qml_file, BasicItem* parent = nullptr);

    QColor filledColor() const;
    void setFilledColor(const QColor& new_filled_color);

    qreal filledOpacity() const;
    void setFilledOpacity(const qreal& new_filled_opacity);

    QColor borderColor() const;
    void setBorderColor(const QColor& new_border_color);

    qreal borderOpacity() const;
    void setBorderOpacity(const qreal& new_border_opacity);

    qreal borderWidth() const;
    void setBorderWidth(const qreal& new_border_width);

    EditableProperties editableProperties() const override;

  signals:
    void filledColorChanged();
    void filledOpacityChanged();
    void borderColorChanged();
    void borderOpacityChanged();
    void borderWidthChanged();

  private:
    QColor m_filled_color = Qt::transparent;
    qreal m_filled_opacity = 1.0;

    QColor m_border_color;
    qreal m_border_opacity = 1.0;
    qreal m_border_width = 4.0;
};

#endif // LIBS_MVA_GUI_INCLUDE_ITEMS_GEOMETRY_ITEM_H_
