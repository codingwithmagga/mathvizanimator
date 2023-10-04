import QtQuick

import cwa.mva.gui

Item {
    id: rootMVARectangle

    property alias item: rectangleItem
    property bool dragActive: false
    property string dragKey: ""
    property bool init: false

    signal clicked(string name)

    width: 100
    height: 100

    Drag.active: dragActive
    Drag.keys: dragKey

    RectangleItem {
        id: rectangleItem

        name: "rect"
        color: "red"

        width: parent.width
        height: parent.height

        MVAMouseArea {
            anchors.fill: parent

            onClicked: {
                rootMVARectangle.clicked(rectangleItem.name)
            }
        }
    }
}