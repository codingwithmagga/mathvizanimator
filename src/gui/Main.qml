import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QtCore

import cwa.mva

ApplicationWindow {
    id: root

    visible: true
    width: 1200
    height: 600
    title: qsTr("mathvizanimator")

    property string thekey: "specialkeyhere"
    property url saveFile: ""
    property var dragItem: null
    property bool objectDragActive: false

    FileDialog {
        id: loadFileDialog
        currentFolder: StandardPaths.standardLocations(
                           StandardPaths.HomeLocation)[0]
        nameFilters: ["JSON (*.json)"]

        onAccepted: {
            newAction.trigger(loadFileDialog)
            main_window.load(selectedFile)
        }
    }

    FileDialog {
        id: saveFileDialog
        currentFolder: StandardPaths.standardLocations(
                           StandardPaths.HomeLocation)[0]
        fileMode: FileDialog.SaveFile
        nameFilters: ["JSON (*.json)"]

        onAccepted: main_window.save(selectedFile, selectArea.objs)
    }

    Popup {
        id: aboutPopup
        anchors.centerIn: parent
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        width: 500

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            Label {
                text: qsTr("MathVizAnimator developed by CodingWithMagga. \ngithub: todo \nWebsite (german): todo \nYouTube (german): todo")
                Layout.alignment: Qt.AlignHCenter

                wrapMode: Text.WordWrap
            }

            Button {
                text: qsTr("Close")
                Layout.alignment: Qt.AlignHCenter

                onClicked: aboutPopup.close()
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                id: newAction

                text: qsTr("&New...")
                onTriggered: {

                    function destroyElement(value) {
                        value.destroy()
                    }

                    mObjectsListModel.clear()
                    selectArea.objs.forEach(destroyElement)
                    selectArea.objs = []
                }
            }
            Action {
                id: loadAction

                text: qsTr("&Open...")
                onTriggered: loadFileDialog.open()
            }
            Action {
                id: saveAction

                text: qsTr("&Save")
                onTriggered: saveFileDialog.open()
            }
            Action {
                id: saveAsAction

                text: qsTr("Save &As...")
                onTriggered: saveFileDialog.open()
            }
            MenuSeparator {}
            Action {
                text: qsTr("&Quit")
            }
        }
        Menu {
            title: qsTr("&Project")
            Action {
                text: qsTr("&Render")

                onTriggered: main_window.buttonClicked(selectArea.objs)
            }
            Action {
                text: qsTr("&Snapshot")
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")

                onTriggered: aboutPopup.open()
            }
        }
    }

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
            objectName: "selectArea"

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
                               drag.source.item.file)

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
