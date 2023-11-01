
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

Item {
    id: rootSurroundingRectangle

    width: parent.width + 2 * highlightRectMargin
    height: parent.height + 2 * highlightRectMargin

    anchors.centerIn: parent

    Rectangle {
        width: parent.width
        height: parent.height

        anchors.centerIn: parent
        border.color: "black"
        color: "transparent"
    }
}
