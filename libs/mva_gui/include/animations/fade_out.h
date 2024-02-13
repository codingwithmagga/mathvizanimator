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

#ifndef LIBS_MVA_GUI_INCLUDE_ANIMATIONS_FADE_OUT_H_
#define LIBS_MVA_GUI_INCLUDE_ANIMATIONS_FADE_OUT_H_

#include "real_property_animation.h"

class FadeOut : public RealPropertyAnimation {
    Q_OBJECT

  public:
    explicit FadeOut(QObject* parent = nullptr);

  private:
    void setStartPropertyValue(qreal new_start_property_value);
    void setEndPropertyValue(qreal new_end_property_value);
};

#endif // LIBS_MVA_GUI_INCLUDE_ANIMATIONS_FADE_OUT_H_
