
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
import QtQuick.Controls 2.15

Item {
    id: delegate
    objectName: "MVAItemDelegate"

    height: 40
    width: parent.width

    required property string image_src
    required property string name
    required property string file

    MouseArea {

        function createShadow(file) {
            var point = mapToItem(null, mouseX, mouseY)

            const component = Qt.createComponent(file)
            if (component.status === Component.Ready) {
                var object = component.createObject(root, {
                                                        "x": point.x - 15,
                                                        "y": point.y - 8,
                                                        "width": 30,
                                                        "height": 16,
                                                        "item.color": "red",
                                                        "dragActive": true,
                                                        "dragKey": thekey
                                                    })
                return object
            } else {
                console.log("Error occured during component creation: " + component.errorString(
                                ))
            }

            return null
        }

        id: dragArea

        anchors.fill: parent
        onPressed: drag.target = createShadow(file)

        onReleased: mouse => {
                        drag.target.Drag.drop()
                        drag.target.destroy()
                    }

        Image {
            id: objectImage
            height: parent.height

            fillMode: Image.PreserveAspectFit
            source: image_src

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }

        Label {
            id: objectText
            height: parent.height

            anchors.left: objectImage.right
            anchors.leftMargin: 10

            verticalAlignment: Text.AlignVCenter

            text: name
            font.bold: true
        }
    }
}
