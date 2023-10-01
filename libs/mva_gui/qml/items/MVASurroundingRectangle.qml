import QtQuick

Item {
    id: rootSurroundingRectangle

    width: parent.width + 2 * highlightRectMargin
    height: parent.height + 2 * highlightRectMargin

    anchors.centerIn: parent

    Rectangle {
        width: parent.width
        height: parent.height

        anchors.centerIn: parent
        border.color: "black"
        color: "transparent"
    }
}
