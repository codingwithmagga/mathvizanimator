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

#ifndef REAL_PROPERTY_ANIMATION_H
#define REAL_PROPERTY_ANIMATION_H

#include "property_animation.h"

class RealPropertyAnimation : public PropertyAnimation {
  Q_OBJECT

  Q_PROPERTY(qreal start_property_value READ startPropertyValue WRITE
                 setStartPropertyValue NOTIFY startPropertyValueChanged)
  Q_PROPERTY(qreal end_property_value READ endPropertyValue WRITE
                 setEndPropertyValue NOTIFY endPropertyValueChanged)

 public:
  explicit RealPropertyAnimation(const QString &property,
                                 QObject *parent = Q_NULLPTR);

  void applyAnimation(AbstractItem *item, const qreal time) const override;

  qreal startPropertyValue() const;
  void setStartPropertyValue(qreal new_start_property_value);

  qreal endPropertyValue() const;
  void setEndPropertyValue(qreal new_end_property_value);

 signals:
  void startPropertyValueChanged();
  void endPropertyValueChanged();

 private:
  qreal calculatePropertyValue(const qreal time) const;

  qreal m_start_property_value = 0.0;
  qreal m_end_property_value = 1.0;
};

#endif  // REAL_PROPERTY_ANIMATION_H
