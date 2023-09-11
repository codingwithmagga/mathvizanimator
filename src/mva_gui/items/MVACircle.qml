import QtQuick

import mva_gui

Item {
    id: circleItem

    property alias item: circle
    property string file: "qrc:/qt/qml/mva_gui/items/MVACircle.qml"
    property bool dragActive: false
    property string dragKey: ""

    width: 100
    height: 100

    Drag.active: dragActive
    Drag.keys: dragKey

    CircleItem {
        id: circle

        name: "circle"
        color: "blue"

        height: parent.width
        width: height

        MouseArea {
            anchors.fill: circle
            onClicked: {
                circle.color = "yellow"
            }
        }
    }
}
