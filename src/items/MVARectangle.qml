import QtQuick

import cwa.mva

Item {
    id: rootMVARectangle

    property alias item: rectangleItem
    property string file: "qrc:/qt/qml/cwa/mva/items/MVARectangle.qml"
    property bool dragActive: false
    property string dragKey: ""

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
        }
    }
}
