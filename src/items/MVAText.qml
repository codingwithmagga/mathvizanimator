import QtQuick

import cwa.mva

Item {
    id: rootMVAText

    property alias item: textItem
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

        MVAMouseArea {
            anchors.fill: parent
        }
    }

    //    Component.onCompleted: {
    //        init(Param1, Param2)
    //    }
}
