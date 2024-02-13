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

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(real_property_animation, "cwa.mva.real_property_animation")

RealPropertyAnimation::RealPropertyAnimation(const QStringList& properties, QObject* parent)
    : PropertyAnimation { properties, parent }
{
}

RealPropertyAnimation::RealPropertyAnimation(const QString& property, QObject* parent)
    : RealPropertyAnimation { QStringList { property }, parent }
{
}

void RealPropertyAnimation::applyAnimation(AbstractItem* item, const qreal time) const
{
    const auto property_list = property();

    if (m_start_property_values.size() != m_end_property_values.size()
        || m_start_property_values.size() != property_list.size()) {
        qCWarning(real_property_animation) << "Size mismatch for property animation:" << m_start_property_values.size()
                                           << m_end_property_values.size() << property_list.size();
        return;
    }

    if (state(time) != State::RUNNING) {
        return;
    }

    for (int i = 0; i < m_start_property_values.size(); ++i) {
        auto property_value = linearApprox(m_start_property_values[i], m_end_property_values[i], time);
        item->setProperty(property_list[i].toUtf8(), property_value);
    }
}

QList<qreal> RealPropertyAnimation::startPropertyValues() const { return m_start_property_values; }

void RealPropertyAnimation::setStartPropertyValues(QList<qreal> new_start_property_values)
{
    if (qFuzzyListCompare(m_start_property_values, new_start_property_values)) {
        return;
    }

    m_start_property_values = new_start_property_values;
    emit startPropertyValuesChanged();
}

QList<qreal> RealPropertyAnimation::endPropertyValues() const { return m_end_property_values; }

void RealPropertyAnimation::setEndPropertyValues(QList<qreal> new_end_property_values)
{
    if (qFuzzyListCompare(m_end_property_values, new_end_property_values)) {
        return;
    }

    m_end_property_values = new_end_property_values;
    emit endPropertyValuesChanged();
}

qreal RealPropertyAnimation::linearApprox(const qreal start_value, const qreal end_value, const qreal time) const
{
    auto property_value = start_value + (time - startTime()) * (end_value - start_value) / duration();

    if (start_value < end_value) {
        property_value = qBound(start_value, property_value, end_value);
    } else {
        property_value = qBound(end_value, property_value, start_value);
    }

    return property_value;
}

bool RealPropertyAnimation::qFuzzyListCompare(const QList<qreal>& first_list, const QList<qreal>& second_list)
{
    if (first_list.size() != second_list.size()) {
        return false;
    }

    for (auto it1 = first_list.begin(), it2 = second_list.begin(); it1 != first_list.end() && it2 != second_list.end();
         ++it1, ++it2) {
        if (!qFuzzyCompare(*it1, *it2)) {
            return false;
        }
    }
    return true;
}
