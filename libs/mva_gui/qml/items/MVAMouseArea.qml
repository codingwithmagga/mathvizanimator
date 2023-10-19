import QtQuick

MouseArea {
    id: mvaMouseArea

    required property var basicItem
    property var highlightRect: null
    property int highlightRectMargin: 10

    onPressed: mouse => {
                   dragActive = true
                   drag.target = basicItem

                   const component = Qt.createComponent(
                       "MVASurroundingRectangle.qml")
                   if (component.status === Component.Ready) {
                       highlightRect = component.createObject(parent, {})
                   } else {
                       console.log(
                           "Error creating Highlight Rectangle while moving item.")
                   }
               }

    onReleased: {
        dragActive = false
        drag.target = null
        highlightRect.destroy()
    }
}
