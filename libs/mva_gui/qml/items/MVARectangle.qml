import QtQuick

import cwa.mva.gui

MVABasicItem {
    id: rootMVARectangle

    property alias item: rectangleItem

    RectangleItem {
        id: rectangleItem

        name: "rect"
        color: "red"

        width: parent.width
        height: parent.height

        MVAMouseArea {
            anchors.fill: parent
            basicItem: rootMVARectangle

            onClicked: {
                rootMVARectangle.clicked(rectangleItem.name)
            }
        }
    }
}
