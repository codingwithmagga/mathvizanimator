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

#include "fade_out.h"

FadeOut::FadeOut(QObject* parent)
    : RealPropertyAnimation { QStringList { "opacity" }, parent }
{
    setStartPropertyValue(1.0);
    setEndPropertyValue(0.0);
}

void FadeOut::setStartPropertyValue(qreal new_start_property_value)
{
    RealPropertyAnimation::setStartPropertyValues(QList<qreal> { new_start_property_value });
}

void FadeOut::setEndPropertyValue(qreal new_end_property_value)
{
    RealPropertyAnimation::setEndPropertyValues(QList<qreal> { new_end_property_value });
}
