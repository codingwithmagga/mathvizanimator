import QtQuick

import cwa.mva

Item {
    id: rootMVACircle

    property alias item: circleItem
    property bool dragActive: false
    property string dragKey: ""
    property bool init: false

    signal clicked(string name)

    width: 100
    height: 100

    Drag.active: dragActive
    Drag.keys: dragKey

    CircleItem {
        id: circleItem

        name: "circle"
        color: "blue"

        height: parent.width
        width: height

        MVAMouseArea {
            anchors.fill: parent

            onClicked: {
                rootMVACircle.clicked(circleItem.name)
            }
        }
    }
}
