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

#include "real_property_animation.h"

RealPropertyAnimation::RealPropertyAnimation(const QString& property,
    QObject* parent)
    : PropertyAnimation { property, parent }
{
}

void RealPropertyAnimation::applyAnimation(AbstractItem* item,
    const qreal time) const
{
    if (state(time) != State::RUNNING) {
        return;
    }

    auto property_value = calculatePropertyValue(time);

    item->setProperty(property().toUtf8(), property_value);
}

qreal RealPropertyAnimation::startPropertyValue() const
{
    return m_start_property_value;
}

void RealPropertyAnimation::setStartPropertyValue(
    qreal new_start_property_value)
{
    if (qFuzzyCompare(m_start_property_value, new_start_property_value)) {
        return;
    }

    m_start_property_value = new_start_property_value;
    emit startPropertyValueChanged();
}

qreal RealPropertyAnimation::endPropertyValue() const
{
    return m_end_property_value;
}

void RealPropertyAnimation::setEndPropertyValue(qreal new_end_property_value)
{
    if (qFuzzyCompare(m_end_property_value, new_end_property_value)) {
        return;
    }

    m_end_property_value = new_end_property_value;
    emit endPropertyValueChanged();
}

qreal RealPropertyAnimation::calculatePropertyValue(const qreal time) const
{
    auto property_value = m_start_property_value + (time - startTime()) * (m_end_property_value - m_start_property_value) / duration();

    if (m_start_property_value < m_end_property_value) {
        property_value = qBound(m_start_property_value, property_value, m_end_property_value);
    } else {
        property_value = qBound(m_end_property_value, property_value, m_start_property_value);
    }

    return property_value;
}
