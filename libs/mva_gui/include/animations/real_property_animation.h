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

#ifndef LIBS_MVA_GUI_INCLUDE_ANIMATIONS_REAL_PROPERTY_ANIMATION_H_
#define LIBS_MVA_GUI_INCLUDE_ANIMATIONS_REAL_PROPERTY_ANIMATION_H_

#include "property_animation.h"

#include <QList>

class RealPropertyAnimation : public PropertyAnimation {
    Q_OBJECT

    Q_PROPERTY(QList<qreal> start_property_values READ startPropertyValues WRITE setStartPropertyValues NOTIFY
            startPropertyValuesChanged)
    Q_PROPERTY(QList<qreal> end_property_values READ endPropertyValues WRITE setEndPropertyValues NOTIFY
            endPropertyValuesChanged)

  public:
    explicit RealPropertyAnimation(const QStringList& properties, QObject* parent = Q_NULLPTR);
    explicit RealPropertyAnimation(const QString& property, QObject* parent = Q_NULLPTR);

    void applyAnimation(AbstractItem* item, const qreal time) const override;

    QList<qreal> startPropertyValues() const;
    void setStartPropertyValues(QList<qreal> new_start_property_values);

    QList<qreal> endPropertyValues() const;
    void setEndPropertyValues(QList<qreal> new_end_property_values);

  signals:
    void startPropertyValuesChanged();
    void endPropertyValuesChanged();

  private:
    qreal linearApprox(const qreal start_value, const qreal end_value, const qreal time) const;
    bool qFuzzyListCompare(const QList<qreal>& first_list, const QList<qreal>& second_list);

    QList<qreal> m_start_property_values { 0.0 };
    QList<qreal> m_end_property_values { 1.0 };
};

#endif // LIBS_MVA_GUI_INCLUDE_ANIMATIONS_REAL_PROPERTY_ANIMATION_H_
