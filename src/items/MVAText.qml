import QtQuick

import cwa.mva

Item {
    id: root

    property alias item: textItem
    property string file: "qrc:/qt/qml/cwa/mva/items/MVAText.qml"
    property bool dragActive: false
    property string dragKey: ""

    Drag.active: dragActive
    Drag.keys: dragKey

    height: 10
    width: 10

    TextItem {
        id: textItem

        name: "text"
        color: "black"

        height: parent.height
        width: parent.width
    }
}
