import QtQuick

import cwa.mva.gui

MVABasicItem {
    id: rootMVACircle

    property alias item: circleItem

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
