import QtQuick

MouseArea {
    id: mvaMouseArea

    property var highlightRect: null
    property int highlightRectMargin: 10

    //onClicked:
    onPressed: mouse => {
                   dragActive = true
                   drag.target = parent

                   const component = Qt.createComponent(
                       "MVASurroundingRectangle.qml")
                   if (component.status === Component.Ready) {
                       highlightRect = component.createObject(parent, {})
                   } else {
                       console.log(
                           "Error creating Highlight Rectangle when moving item.")
                   }
               }

    onReleased: {
        dragActive = false
        highlightRect.destroy()
    }
}
