import QtQuick

import mva_gui

Item {
    id: rectangleItem

    property alias item: rectangle
    property string file: "qrc:/qt/qml/mva_gui/items/MVARectangle.qml"
    property bool dragActive: false
    property string dragKey: ""

    width: 100
    height: 100

    Drag.active: dragActive
    Drag.keys: dragKey

    RectangleItem {
        id: rectangle

        name: "rect"
        color: "red"

        width: parent.width
        height: parent.height

        MouseArea {
            anchors.fill: rectangle
            onClicked: {
                rectangle.color = "blue"
            }
        }
    }
}
