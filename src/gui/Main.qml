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

    Shortcut {
        sequences: [StandardKey.Delete]
        onActivated: {
            mObjectsListModel.remove(mObjectsListView.currentIndex)

            // only works first time
            selectArea.objs[mObjectsListView.currentIndex].destroy()
        }
    }

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
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

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
        anchors.leftMargin: 0
        anchors.rightMargin: 0

        background: Rectangle {
            color: palette.dark
        }

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
                onTriggered: root.close()
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

            signal itemAdded(Item item)

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
                                       "y": drag.y,
                                       "init": true
                                   })

                               itemAdded(abstractItem)
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

        Rectangle {
            color: palette.dark
            Layout.minimumWidth: 150
            Layout.preferredWidth: 300
            Layout.maximumWidth: 400
            Layout.minimumHeight: 150
            Layout.fillHeight: true
            Layout.margins: 20

            TableView {
                id: mObjectsListView

                anchors.fill: parent

                rowSpacing: 4
                columnSpacing: 2
                anchors.topMargin: horizontalHeader.implicitHeight

                columnWidthProvider: function (column) {
                    return mObjectsListView.width / 2
                }

                model: item_model

                selectionModel: ItemSelectionModel {
                    id: selectionModel

                    // Seems to create a bug when closing the app  QObject::disconnect: No such signal ...
                    // maybe happens when using the same model twice?
                    model: item_model
                }

                delegate: Label {
                    required property bool current

                    id: delegateLabel

                    text: display
                    padding: 10

                    color: row === mObjectsListView.currentRow ? palette.highlightedText : palette.text
                    background: Rectangle {
                        anchors.fill: parent

                        color: row === mObjectsListView.currentRow ? palette.highlight : palette.dark
                    }
                }

                onCurrentRowChanged: console.log("current row changed")
            }

            HorizontalHeaderView {
                id: horizontalHeader
                syncView: mObjectsListView
                anchors.left: mObjectsListView.left
                anchors.top: parent.top
                clip: true
            }
        }
    }
}
