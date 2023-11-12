
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

MVABasicItem {
    id: rootMVAPaintedCircle

    property alias item: circlePaintedItem

    CirclePaintedItem {
        id: circlePaintedItem

        name: "circle"
        color: "blue"

        height: parent.width
        width: height

        MVAMouseArea {
            anchors.fill: parent
            basicItem: rootMVAPaintedCircle

            onClicked: {
                rootMVAPaintedCircle.clicked(circlePaintedItem.name)
                console.log(rootMVAPaintedCircle.layer.samples)
            }
        }
    }
}
