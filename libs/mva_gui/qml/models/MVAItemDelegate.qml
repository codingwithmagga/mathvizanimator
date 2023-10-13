import QtQuick
import QtQuick.Controls 2.15

Item {
    id: delegate
    height: 40
    width: parent.width

    required property string image_src
    required property string name
    required property string file

    MouseArea {

        function createShadow(file) {
            var point = mapToItem(null, mouseX, mouseY)

            console.log("file: " + file)

            const component = Qt.createComponent(file)
            if (component.status === Component.Ready) {
                var object = component.createObject(root, {
                                                        "x": point.x - 15,
                                                        "y": point.y - 8,
                                                        "width": 30,
                                                        "height": 16,
                                                        "item.color": "red",
                                                        "dragActive": true,
                                                        "dragKey": thekey
                                                    })
                return object
            } else {
                console.log("Error creat com")
            }

            return null
        }

        id: dragArea

        anchors.fill: parent
        onPressed:  drag.target = createShadow(file)

        onReleased: mouse => {
                        drag.target.Drag.drop()
                        drag.target.destroy()
                    }

        Image {
            id: objectImage
            height: parent.height

            fillMode: Image.PreserveAspectFit
            source: image_src

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
        }

        Label {
            id: objectText
            height: parent.height

            anchors.left: objectImage.right
            anchors.leftMargin: 10

            verticalAlignment: Text.AlignVCenter

            text: name
            font.bold: true
        }
    }
}
