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
            basicItem: rootMVACircle

            onClicked: {
                rootMVACircle.clicked(circleItem.name)
            }
        }
    }
}
