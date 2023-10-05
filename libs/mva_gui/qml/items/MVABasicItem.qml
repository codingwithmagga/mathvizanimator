import QtQuick

Item {
    id: rootItem

    property bool dragActive: false
    property string dragKey: ""
    property bool init: false

    signal clicked(string name)

    width: 100
    height: 100

    Drag.active: dragActive
    Drag.keys: dragKey
}
