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

#ifndef LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEM_OBSERVER_H_
#define LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEM_OBSERVER_H_

#include <QObject>
#include <QVariantMap>

#include "abstract_animation.h"
#include "basic_item.h"

class ItemObserver : public QObject {
  public:
    explicit ItemObserver(BasicItem* const item, QObject* parent = nullptr);

    void setTimeProgressive(const qreal time);
    void setTime(const qreal time);

    void addAnimation(const QSharedPointer<AbstractAnimation>& animation);
    void addAnimations(const QList<QSharedPointer<AbstractAnimation>>& animations);
    void removeAnimation(const QSharedPointer<AbstractAnimation>& animation);
    void removeAnimation(const qint32 animation_number);

    void updateItemProperty(const QString& property, const QVariant& value);

    QJsonObject toJson() const;

    BasicItem* item() const;
    AbstractItem* abstractitem() const;

    QList<QSharedPointer<AbstractAnimation>> animations() const;

  private:
    void sortAnimations();
    void applyStartProperties();

    BasicItem* m_item;
    QList<QSharedPointer<AbstractAnimation>> m_animations;

    QVariantMap m_item_start_property_values;
    QVariantMap m_basic_item_start_property_values;
};

inline BasicItem* ItemObserver::item() const { return m_item; }

inline AbstractItem* ItemObserver::abstractitem() const { return m_item->abstractItem(); }

inline QList<QSharedPointer<AbstractAnimation>> ItemObserver::animations() const { return m_animations; }

#endif // LIBS_MVA_WORKFLOW_INCLUDE_WORKFLOW_ITEM_OBSERVER_H_
