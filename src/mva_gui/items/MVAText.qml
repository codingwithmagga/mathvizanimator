import QtQuick

import mva_gui

Item {
    id: textItem

    property alias item: text
    property string file: "qrc:/qt/qml/mva_gui/items/MVAText.qml"
    property bool dragActive: false
    property string dragKey: ""

    Drag.active: dragActive
    Drag.keys: dragKey

    height: 10
    width: 10

    TextItem {
        id: text

        name: "text"
        color: "black"

        height: parent.height
        width: parent.width
    }
}
