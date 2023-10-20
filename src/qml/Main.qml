import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import QtCore

import cwa.mva.gui

ApplicationWindow {
    id: root

    visible: true
    width: 1400
    height: 600
    title: qsTr("mathvizanimator")

    minimumHeight: 500
    minimumWidth: 1200

    property ApplicationWindow appWindow: root
    property string thekey: "specialkeyhere"
    property url saveFile: ""
    property var dragItem: null
    property bool objectDragActive: false

    Shortcut {
        sequences: [StandardKey.Delete]
        onActivated: {
            main_window.removeRow(mObjectsListView.currentRow)
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

        onAccepted: main_window.save(selectedFile)
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
                    main_window.clearAllItems()
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

                onTriggered: main_window.render()
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
            Layout.preferredWidth: 100
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

        Rectangle {
            id: dropRectangle

            Layout.minimumWidth: 600
            Layout.minimumHeight: 400
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 20

            color: palette.dark

            // TODO: Put this in a reusable component
            Item {
                id: dropAreaContainer

                property real aimedRatio: 768 / 1024

                property bool parentIsLarge: parentRatio > aimedRatio
                property double parentRatio: parent.height / parent.width

                anchors.horizontalCenter: dropRectangle.horizontalCenter
                anchors.verticalCenter: dropRectangle.verticalCenter

                height: parentIsLarge ? width * aimedRatio : parent.height
                width: parentIsLarge ? parent.width : height / aimedRatio

                DropArea {
                    id: creationArea
                    objectName: "creationArea"

                    property var abstractItem: null
                    property var abstractComponent: null

                    property double baseWidth: 1024
                    property double baseHeight: baseWidth * dropAreaContainer.aimedRatio
                    property double scaleFactor: parent.width / baseWidth

                    signal itemAdded(Item item)

                    anchors.centerIn: parent

                    width: baseWidth
                    height: baseHeight
                    scale: scaleFactor

                    clip: true

                    Drag.keys: [root.thekey]

                    onDropped: drop => {
                                   function itemClicked(itemName) {
                                       var row = main_window.getRowByItemName(
                                                   itemName)
                                       selectionModel.setCurrentIndex(
                                                   selectionModel.model.index(
                                                       row, 0),
                                                   ItemSelectionModel.Current)
                                   }

                                   drop.accept(Qt.MoveAction)

                                   const component = Qt.createComponent(
                                       drag.source.item.file)

                                   if (component.status === Component.Ready) {

                                       abstractItem = component.createObject(
                                           creationArea, {
                                               "x": drag.x,
                                               "y": drag.y,
                                               "init": true
                                           })

                                       abstractItem.clicked.connect(itemClicked)

                                       itemAdded(abstractItem)
                                   } else {
                                       console.log("Error creating object")
                                   }
                               }

                    Rectangle {

                        id: dropZone

                        anchors.fill: parent
                        color: "white"
                    }

                    TapHandler {
                        onTapped: {
                            selectionModel.setCurrentIndex(
                                        selectionModel.model.index(-1, 0),
                                        ItemSelectionModel.Current)
                        }
                    }
                }
            }
        }

        Rectangle {
            id: mObjectsListViewRect

            color: palette.dark
            Layout.minimumWidth: 150
            Layout.preferredWidth: 300
            Layout.maximumWidth: 400
            Layout.minimumHeight: 200
            Layout.fillHeight: true
            Layout.margins: 10

            ColumnLayout {

                spacing: 0

                HorizontalHeaderView {
                    id: horizontalHeader
                    syncView: mObjectsListView
                    Layout.alignment: Qt.AlignTop
                    clip: true
                }

                ScrollView {
                    id: mObjectsListViewScrollView

                    Layout.preferredHeight: (mObjectsListViewRect.height
                                             - horizontalHeader.implicitHeight
                                             - hHeaderPropTable.implicitHeight) / 2
                    Layout.preferredWidth: mObjectsListViewRect.width
                    Layout.maximumWidth: mObjectsListViewRect.width
                    Layout.minimumHeight: 100
                    Layout.alignment: Qt.AlignTop
                    Layout.fillHeight: true

                    contentWidth: mObjectsListView.width
                    contentHeight: mObjectsListView.height

                    ScrollBar.horizontal.policy: mObjectsListView.contentWidth > mObjectsListView.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: mObjectsListView.contentHeight > mObjectsListView.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff

                    TableView {
                        id: mObjectsListView

                        width: mObjectsListViewRect.width
                        height: mObjectsListViewRect.height / 2

                        alternatingRows: true
                        clip: true

                        columnWidthProvider: function (column) {
                            return mObjectsListView.model ? mObjectsListView.width
                                                            / mObjectsListView.model.columnCount(
                                                                ) : 0
                        }

                        model: item_model

                        selectionModel: ItemSelectionModel {
                            id: selectionModel

                            // Seems to create a bug when closing the app  QObject::disconnect: No such signal ...
                            // maybe happens when using the same model twice? Watched on linux, windows don't have this problem...
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

                                color: row === mObjectsListView.currentRow ? palette.highlight : (mObjectsListView.alternatingRows && row % 2 !== 0 ? palette.alternateBase : palette.base)
                            }
                        }

                        onCurrentRowChanged: {
                            main_window.currentRowChanged(currentRow)
                        }
                    }
                }

                HorizontalHeaderView {
                    id: hHeaderPropTable
                    syncView: mPropertyTable
                    Layout.alignment: Qt.AlignTop
                    clip: true
                }

                ScrollView {
                    id: mPropertyTableScrollView

                    Layout.preferredHeight: (mObjectsListViewRect.height
                                             - horizontalHeader.implicitHeight
                                             - hHeaderPropTable.implicitHeight) / 2
                    Layout.preferredWidth: mObjectsListViewRect.width
                    Layout.maximumWidth: mObjectsListViewRect.width
                    Layout.minimumHeight: 100
                    Layout.alignment: Qt.AlignTop
                    Layout.fillHeight: true

                    contentWidth: mPropertyTable.width
                    contentHeight: mPropertyTable.height

                    ScrollBar.horizontal.policy: mPropertyTable.contentWidth > mPropertyTable.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: mPropertyTable.contentHeight > mPropertyTable.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff

                    TableView {
                        id: mPropertyTable

                        alternatingRows: true
                        clip: true

                        columnWidthProvider: function (column) {
                            return mPropertyTable.model ? mPropertyTable.width
                                                          / mPropertyTable.model.columnCount(
                                                              ) : 0
                        }

                        model: property_model

                        selectionModel: ItemSelectionModel {
                            id: mPropertyTableSelectionModel

                            // Seems to create a bug when closing the app  QObject::disconnect: No such signal ...
                            // maybe happens when using the same model twice? Watched on linux, windows don't have this problem...
                            model: property_model
                        }

                        delegate: Label {
                            required property bool current

                            id: mPropertyTableDelegate

                            text: display
                            padding: 10

                            color: row === mPropertyTable.currentRow ? palette.highlightedText : palette.text
                            background: Rectangle {
                                anchors.fill: parent

                                color: row === mPropertyTable.currentRow ? palette.highlight : (mPropertyTable.alternatingRows && row % 2 !== 0 ? palette.alternateBase : palette.base)
                            }

                            TableView.editDelegate: TextField {
                                anchors.fill: parent
                                text: display
                                horizontalAlignment: TextInput.AlignHCenter
                                verticalAlignment: TextInput.AlignVCenter
                                Component.onCompleted: selectAll()

                                TableView.onCommit: {
                                    display = text
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
