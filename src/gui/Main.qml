import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

import cwa.mva

ApplicationWindow {
    id: root

    visible: true
    width: 1200
    height: 600
    title: qsTr("mathvizanimator")

    property string thekey: "specialkeyhere"
    property var dragItem: null
    property bool objectDragActive: false

    RowLayout {
        anchors.fill: parent

        spacing: 5

        ListView {

            id: itemView

            Layout.minimumWidth: 50
            Layout.preferredWidth: 150
            Layout.maximumWidth: 300
            Layout.minimumHeight: 150
            Layout.fillHeight: true
            Layout.margins: 20

            z: 1

            model: MVAItemModel {}

            delegate: MVAItemDelegate {
                id: delegate
                width: parent.width
            }
        }

        DropArea {
            id: selectArea

            Layout.minimumWidth: 600
            Layout.minimumHeight: 400
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20

            property var abstractItem: null
            property var abstractComponent: null
            property var objs: []

            clip: true

            Drag.keys: [root.thekey]

            onDropped: drop => {
                           drop.accept(Qt.MoveAction)

                           const component = Qt.createComponent(
                               drag.source.file)

                           if (component.status === Component.Ready) {

                               abstractItem = component.createObject(
                                   selectArea, {
                                       "x": drag.x,
                                       "y": drag.y
                                   })

                               mObjectsListModel.append({
                                                            "name": abstractItem.item.name
                                                        })
                               objs.push(abstractItem.item)
                           } else {
                               console.log("Error creating object")
                           }
                       }

            Rectangle {

                id: dropZone

                anchors.fill: parent
                color: "white"
            }
        }

        ColumnLayout {

            spacing: 5

            Layout.minimumWidth: 50
            Layout.preferredWidth: 100
            Layout.maximumWidth: 100
            Layout.minimumHeight: 150
            Layout.fillHeight: true
            Layout.margins: 20

            Button {
                text: "Render"
                onClicked: main_window.buttonClicked(selectArea.objs)
            }
        }

        ListView {
            id: mObjectsListView

            Layout.minimumWidth: 150
            Layout.preferredWidth: 200
            Layout.maximumWidth: 300
            Layout.minimumHeight: 150
            Layout.fillHeight: true
            Layout.margins: 20

            model: ListModel {
                id: mObjectsListModel
            }

            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
            focus: true

            delegate: Label {

                text: 'Name: ' + name

                MouseArea {
                    id: mObjectsListViewItemMouseArea
                    anchors.fill: parent

                    onClicked: mObjectsListView.currentIndex = index
                }
            }

            onCurrentIndexChanged: console.log("current index changed")
        }
    }
}
