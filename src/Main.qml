import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import mathvizanimator
import mva_gui

ApplicationWindow {
    id: root

    visible: true
    width: 1200
    height: 600
    title: qsTr("mathvizanimator")

    property string thekey: "specialkeyhere"
    property var dragItem: null
    property bool objectDragActive: false

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        spacing: 5

        ListView {

            id: itemView

            width: 150
            height: parent.height
            z: 1

            model: ItemModel {}

            delegate: Item {
                id: delegate
                height: 40
                width: itemView.width

                MouseArea {

                    function createShadow(file, mouse) {
                        var point = mapToItem(root, mouseX, mouseY)

                        console.log(file)

                        const component = Qt.createComponent(file)
                        if (component.status === Component.Ready) {
                            var object = component.createObject(root, {
                                                                    "x": point.x - 15,
                                                                    "y": point.y - 8,
                                                                    "width": 30,
                                                                    "height": 16,
                                                                    "item.color": "red",
                                                                    "dragActive": objectDragActive,
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
                    onPressed: mouse => {
                                   itemView.currentIndex = index

                                   objectDragActive = true

                                   drag.target = createShadow(file, mouse)
                               }

                    onReleased: mouse => {
                                    drag.target.Drag.drop()

                                    objectDragActive = false
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
        }

        MObjectModel {
            id: mObjectModel
        }

        Component {
            id: objectDelegate
            Item {
                width: 180
                height: 40
                Column {
                    Text {
                        text: '<b>Name:</b> ' + color
                    }
                }
            }
        }

        DropArea {
            id: selectArea

            width: 600
            height: 400

            property var abstractItem: null
            property var abstractComponent: null
            property var objs: []

            Drag.keys: [root.thekey]

            onDropped: drop => {
                           drop.accept(Qt.MoveAction)

                           const component = Qt.createComponent(
                               drag.source.file)
                           console.log("dropped file " + drag.source.file)
                           if (component.status === Component.Ready) {
                               console.log(
                                   "dropped! at " + drag.x + " " + drag.y)

                               abstractItem = component.createObject(
                                   selectArea, {
                                       "x": drag.x,
                                       "y": drag.y
                                   })

                               console.log(
                                   "object created at " + drag.x + " " + drag.y)
                               mObjectModel.addMObject(abstractItem.item)
                               objs.push(abstractItem.item)
                           } else {
                               console.log("Error creating object")
                           }
                       }

            Rectangle {

                id: dropZone

                anchors.fill: parent
                color: "black"
            }
        }
        ColumnLayout {

            spacing: 5
            Button {
                text: "Render"
                onClicked: main_window.buttonClicked(selectArea.objs)
            }
        }

        Component {
            id: contactDelegate
            Item {
                width: 180
                height: 40
                Column {
                    Text {
                        text: '<b>Name:</b> ' + name
                    }
                    Text {
                        text: '<b>Number:</b> ' + index
                    }
                }
            }
        }

        ListView {
            id: mObjectsListView
            width: 200
            height: parent.height
            model: mObjectModel
            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
            focus: true

            delegate: Text {
                text: 'Name: ' + mObjectModel.data(mObjectModel.index(index,
                                                                      0), 0)
            }
        }
    }
}
