
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

MouseArea {
    id: mvaMouseArea

    required property var basicItem
    property var highlightRect: null
    property int highlightRectMargin: 10

    onPressed: mouse => {
                   dragActive = true
                   drag.target = basicItem

                   const component = Qt.createComponent(
                       "MVASurroundingRectangle.qml")
                   if (component.status === Component.Ready) {
                       highlightRect = component.createObject(parent, {})
                   } else {
                       console.log(
                           "Error creating Highlight Rectangle while moving item.")
                   }
               }

    onReleased: {
        dragActive = false
        drag.target = null
        highlightRect.destroy()
    }
}
