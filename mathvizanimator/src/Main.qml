import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import mathvizanimator

Window {
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

        Column {
            Repeater {

                id: itemView

                width: 150
                height: parent.height
                z: 1

                property bool held: false

                model: ItemModel {
                    id: model
                }

                delegate: Item {
                    id: delegate
                    height: 70
                    width: itemView.width

                    MouseArea {

                        function createShadow(mouse) {

                            //                            var component = Qt.createQmlObject(
                            //                                        'import QtQuick 2.4; Rectangle{x:100;y:100;width:100;height:100;opacity:0.2;color:"red";Drag.active: dragArea.drag.active;Drag.keys: [root.thekey];}',
                            //                                        root, "asdf")
                            var component = rectangleComponent.createObject(
                                        root, {
                                            "x": mouseX,
                                            "y": mouseY
                                        })

                            console.log(mouse.x)
                            console.log(mouse.y)
                            console.log(mouseX)
                            console.log(mouseY)
                            return component
                        }

                        id: dragArea

                        anchors.fill: parent
                        onPressed: mouse => {
                                       objectDragActive = true
                                       // itemView.currentIndex = index
                                       root.dragItem = createShadow(mouse)
                                       drag.target = root.dragItem
                                   }

                        onReleased: mouse => {
                                        onPressed: console.log(
                                                       "release at " + mouseX + ", " + mouseY)
                                        drag.target.Drag.drop()

                                        objectDragActive = false
                                        root.dragItem.destroy()
                                    }

                        Image {
                            id: objectImage
                            height: 60
                            fillMode: Image.PreserveAspectFit
                            source: image_src

                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.bottom: parent.bottom
                        }

                        Text {
                            id: objectText

                            anchors.left: objectImage.right
                            anchors.verticalCenter: objectImage.verticalCenter
                            anchors.leftMargin: 10

                            text: name
                            font.bold: true
                        }
                    }
                }
            }

            //            onCurrentItemChanged: console.log(
            //                                      model.get(
            //                                          itemView.currentIndex).name + ' selected')
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

            onDropped: {
                console.log("dropped! at " + drag.x + " " + drag.y)
                if (rectangleComponent.status === Component.Ready) {
                    selectArea.abstractItem = rectangleComponent.createObject(
                                selectArea, {
                                    "x": drag.x,
                                    "y": drag.y
                                })
                    console.log("object created at " + drag.x + " " + drag.y)
                } else {
                    console.log("Error loading component:",
                                abstractComponent.errorString())
                }
            }

            Rectangle {

                id: dropZone

                anchors.fill: parent
                color: "black"
            }

            //            MouseArea {
            //                onClicked: {
            //                    console.log(abstractItem.x + " " + abstractItem.y)
            //                }
            //            }

            //            onPressed: mouse => {
            //                           if (abstractComponent.status === Component.Ready) {
            //                               abstractItem = abstractComponent.createObject(
            //                                   selectArea, {
            //                                       "x": mouseX,
            //                                       "y": mouseY
            //                                   })
            //                               if (abstractItem == null) {
            //                                   // Error Handling
            //                                   console.log("Error creating object")
            //                               }
            //                           } else if (abstractComponent.status === Component.Error) {
            //                               // Error Handling
            //                               console.log("Error loading component:",
            //                                           abstractComponent.errorString())
            //                           }
            //                           abstractItem.onPressed(mouseX, mouseY)
            //                       }

            //            onPositionChanged: mouse => {
            //                                   abstractItem.onPosChanged(mouseX, mouseY)
            //                               }
            //            onReleased: {
            //                mObjectModel.addMObject(abstractItem.getRectangleItem())
            //            }
            Component {

                id: rectangleComponent

                Item {

                    id: rectangleItem

                    Drag.active: root.objectDragActive
                    Drag.keys: [root.thekey]

                    //                    function onPressed(mouseX, mouseY) {
                    //                        rectangle.onPressed(mouseX, mouseY)
                    //                    }

                    //                    function onPosChanged(mouseX, mouseY) {
                    //                        rectangle.onPosChanged(mouseX, mouseY)
                    //                    }

                    //                    function getRectangleItem() {
                    //                        return rectangle
                    //                    }
                    RectangleItem {
                        id: rectangle
                        color: "blue"
                        name: "rect"

                        width: 100
                        height: 100

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                rectangle.color = "blue"
                                console.log("pos: " + rectangleItem.x + " " + rectangleItem.y)
                            }
                        }
                    }
                }
            }

            Component {
                id: circleComponent
                Item {
                    id: circleItem

                    function onPressed(mouseX, mouseY) {
                        circle.onPressed(mouseX, mouseY)
                    }

                    function onPosChanged(mouseX, mouseY) {
                        circle.onPosChanged(mouseX, mouseY)
                    }

                    Rectangle {

                        id: circle

                        color: "blue"
                        border.width: 1
                        border.color: "#fff"
                        radius: width * 0.5
                        height: width

                        property string objectName: "Circle"
                        property real startX
                        property real startY

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                color = "yellow"
                            }
                        }

                        function onPressed(mouseX, mouseY) {
                            startX = mouseX
                            startY = mouseY
                        }

                        function onPosChanged(mouseX, mouseY) {

                            if (mouseX - startX < 0) {
                                circleItem.x = mouseX
                            }

                            if (mouseY - startY < 0) {
                                circleItem.y = mouseY
                            }
                            width = (Math.abs(mouseX - startX))
                        }

                        function getPoints() {
                            var arr = []
                            arr.push(2, x, y, width, height)

                            return arr
                        }
                    }
                }
            }

            //                Item {
            //                    id: name

            //                    width: 600
            //                    height: 400
            //                    Image {
            //                        id: image
            //                        source: "image://imageprovider/scene"
            //                        cache: false
            //                        function reload() {
            //                            var oldSource = source
            //                            source = ""
            //                            source = oldSource
            //                            console.log("reload")
            //                        }
            //                    }
            //                }
        }

        ColumnLayout {

            spacing: 5
            Button {
                text: "Render"
                onClicked: main_window.buttonClicked(selectArea.objs)
            }
            Button {
                id: rectangleButton
                text: "Rectangle"
                onClicked: {
                    highlighted = true
                    circleButton.highlighted = false
                    selectArea.abstractComponent = rectangleComponent
                }
            }
            Button {
                id: circleButton
                text: "Circle"
                onClicked: {
                    highlighted = true
                    rectangleButton.highlighted = false
                    selectArea.abstractComponent = circleComponent
                }
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
