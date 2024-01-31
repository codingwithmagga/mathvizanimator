
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
import QtQuick

import cwa.mva.gui

BasicItem {
    id: rootMVACircle

    abstract_item: circleItem

    CircleItem {
        id: circleItem

        name: "circle"
        color: "blue"

        anchors.fill: parent

        MVAMouseArea {
            anchors.fill: parent
            basicItem: rootMVACircle

            onLeftClicked: {
                rootMVACircle.clicked(circleItem.name)
            }

            onAnimationAdded: (type, start_time, duration) => {
                                  rootMVACircle.animationAdded(circleItem.name,
                                                               type,
                                                               start_time,
                                                               duration)
                              }
        }
    }
}
