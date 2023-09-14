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

        MObjectModel {
            id: mObjectModel
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
