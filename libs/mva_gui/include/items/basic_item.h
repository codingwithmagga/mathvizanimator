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

#ifndef LIBS_MVA_GUI_INCLUDE_ITEMS_BASIC_ITEM_H_
#define LIBS_MVA_GUI_INCLUDE_ITEMS_BASIC_ITEM_H_

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QString>

class AbstractItem;

class BasicItem : public QQuickItem {

    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool is_initialized MEMBER m_is_initialized NOTIFY isInitializedChanged)
    Q_PROPERTY(AbstractItem* abstract_item READ abstractItem WRITE setAbstractItem NOTIFY abstractItemChanged REQUIRED)

  public:
    explicit BasicItem(QQuickItem* parent = nullptr);

    inline AbstractItem* abstractItem() const { return m_abstract_item; }
    void setAbstractItem(AbstractItem* const new_item);

    QString abstractItemName() const;

  signals:
    void clicked(const QString& name);
    void animationAdded(const QString& name, const QString& type, const qreal start_time, const qreal duration);

    void isInitializedChanged();
    void abstractItemChanged();

  private:
    AbstractItem* m_abstract_item = nullptr;
    bool m_is_initialized = false;
};

#endif // LIBS_MVA_GUI_INCLUDE_ITEMS_BASIC_ITEM_H_
