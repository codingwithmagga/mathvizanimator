
/* mathvizanimator
 * Copyright (C) 2023 codingwithmagga
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

import QtCore

import cwa.mva.gui

ApplicationWindow {
    id: root

    visible: true
    width: 1600
    height: 1200
    title: qsTr("mathvizanimator")

    font.pointSize: 14

    Material.theme: Material.System

    minimumHeight: 500
    minimumWidth: 1200

    property ApplicationWindow appWindow: root
    property string thekey: "specialkeyhere"
    property url saveFile: ""
    property var dragItem: null
    property bool objectDragActive: false

    signal renderingVideoFinished

    Shortcut {
        sequences: [StandardKey.Delete]
        onActivated: {
            main_window.removeCurrentItem()
        }
    }

    FileDialog {
        id: loadFileDialog
        objectName: "MVALoadFileDialog"

        currentFolder: StandardPaths.standardLocations(
                           StandardPaths.HomeLocation)[0]
        fileMode: FileDialog.OpenFile
        nameFilters: ["JSON (*.json)"]

        onAccepted: {
            newAction.trigger(loadFileDialog)
            main_window.loadProject(selectedFile)
        }

        // Necessary for integration testing, close() is needed for macOS test, it crashes (segmentation fault) otherwise
        function simulateAccepted() {
            accepted()
            close()
        }
    }

    FileDialog {
        id: saveFileDialog
        objectName: "MVASaveFileDialog"

        currentFolder: StandardPaths.standardLocations(
                           StandardPaths.HomeLocation)[0]
        fileMode: FileDialog.SaveFile
        nameFilters: ["JSON (*.json)"]

        onAccepted: main_window.saveProject(selectedFile)

        // Necessary for integration testing, close() is needed for macOS test, it crashes (segmentation fault) otherwise
        function simulateAccepted() {
            accepted()
            close()
        }
    }

    FileDialog {
        id: renderFileDialog
        objectName: "MVARenderFileDialog"

        currentFolder: StandardPaths.standardLocations(
                           StandardPaths.HomeLocation)[0]
        fileMode: FileDialog.SaveFile
        nameFilters: ["MP4 (*.mp4)"]

        onAccepted: main_window.render(selectedFile)

        // Necessary for integration testing, close() is needed for macOS test, it crashes (segmentation fault) otherwise
        function simulateAccepted() {
            accepted()
            close()
        }
    }

    FileDialog {
        id: snapshotFileDialog
        objectName: "MVASnapshotFileDialog"

        currentFolder: StandardPaths.standardLocations(
                           StandardPaths.HomeLocation)[0]
        fileMode: FileDialog.SaveFile
        nameFilters: ["PNG (*.png)"]

        onAccepted: main_window.snapshot(selectedFile)

        // Necessary for integration testing, close() is needed for macOS test, it crashes (segmentation fault) otherwise
        function simulateAccepted() {
            accepted()
            close()
        }
    }

    Dialog {
        id: aboutDialog
        anchors.centerIn: parent

        standardButtons: Dialog.Ok

        title: qsTr("MathVizAnimator 0.0.1")

        ColumnLayout {
            spacing: 25
            width: parent.width
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: qsTr("MathVizAnimator developed by CodingWithMagga. \nThe links below may be interesting for you.")
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter

                width: parent.width

                wrapMode: Text.WordWrap
            }

            Label {
                text: qsTr('<a href="https://github.com/codingwithmagga/mathvizanimator">github</a> (english) <br>\
<a href="https://codingwithmagga.com">Website</a> (german) <br>\
<a href="https://www.youtube.com/channel/UCjlzef-PolOD__Q1VMWznqw">YouTube</a> (german) ')

                Layout.alignment: Qt.AlignHCenter

                onLinkActivated: function (link) {
                    Qt.openUrlExternally(link)
                }
            }
        }
    }

    Dialog {
        id: projectSettingsDialog
        objectName: "MVAProjectSettingsDialog"

        anchors.centerIn: parent
        padding: 12

        standardButtons: Dialog.Ok | Dialog.Cancel

        title: qsTr("Project Settings")

        property list<int> projectData: [widthInputField.text, heightInputField.text, fpsInputField.text, videoLengthInputField.text]

        // Necessary for integration testing, close() is needed for macOS test, it crashes (segmentation fault) otherwise
        function simulateAccepted() {
            accepted()
            close()
        }

        ColumnLayout {
            id: mainLayout

            anchors.fill: parent
            Layout.alignment: Qt.AlignHCenter

            spacing: 40

            GridLayout {

                columns: 2

                rowSpacing: 20
                columnSpacing: 20

                Label {
                    text: qsTr("Width (px):")

                    Layout.alignment: Qt.AlignRight

                    horizontalAlignment: Text.AlignRight
                }

                TextField {
                    id: widthInputField
                    objectName: "MVAWidthInputField"

                    text: main_window.pixel_width

                    horizontalAlignment: TextInput.AlignRight

                    validator: IntValidator {
                        bottom: 50
                        top: 4096
                    }
                }

                Label {
                    text: qsTr("Height (px):")

                    Layout.alignment: Qt.AlignRight
                    horizontalAlignment: Text.AlignRight
                }

                TextField {
                    id: heightInputField
                    objectName: "MVAHeightInputField"

                    text: main_window.pixel_height

                    horizontalAlignment: TextInput.AlignRight

                    validator: IntValidator {
                        bottom: 50
                        top: 2160
                    }
                }

                Label {
                    text: qsTr("Frames per Second (fps):")

                    Layout.alignment: Qt.AlignRight

                    horizontalAlignment: Text.AlignRight
                }

                TextField {
                    id: fpsInputField
                    objectName: "MVAFpsInputField"

                    text: main_window.fps

                    horizontalAlignment: TextInput.AlignRight

                    validator: IntValidator {
                        bottom: 1
                        top: 120
                    }
                }

                Label {
                    text: qsTr("Video length (s):")

                    Layout.alignment: Qt.AlignRight

                    horizontalAlignment: Text.AlignRight
                }

                TextField {
                    id: videoLengthInputField
                    objectName: "MVAVideoLengthInputField"

                    text: main_window.video_length

                    horizontalAlignment: TextInput.AlignRight

                    validator: IntValidator {
                        bottom: 1
                        top: 9999
                    }
                }
            }
        }
        // TODO(codingwithmagga): Validate data before sending, validators themselves are not enough
        onAccepted: {
            main_window.updateProjectSettings(projectSettingsDialog.projectData)
            projectSettingsDialog.close()
        }
    }

    menuBar: MenuBar {
        id: main_menu_bar

        anchors.leftMargin: 0
        anchors.rightMargin: 0

        Menu {
            id: file_menu
            objectName: "MVAFileMenu"

            title: qsTr("&File")

            Action {
                id: newAction
                objectName: "MVANewProjectAction"

                text: qsTr("&New Project")
                onTriggered: {
                    main_window.newProject()
                }
            }
            Action {
                id: loadAction
                objectName: "MVALoadProjectAction"

                text: qsTr("&Open...")
                onTriggered: loadFileDialog.open()
            }

            Action {
                id: saveAsAction
                objectName: "MVASaveProjectAsAction"

                text: qsTr("Save &As...")
                onTriggered: saveFileDialog.open()
            }

            MenuSeparator {}
            Action {
                id: quitAppAction
                objectName: "MVAQuitAppAction"

                text: qsTr("&Quit")
                onTriggered: root.close()
            }
        }
        Menu {
            title: qsTr("&Project")
            Action {
                id: renderProjectAction
                objectName: "MVARenderProjectAction"

                text: qsTr("&Render")
                onTriggered: renderFileDialog.open()
            }
            Action {
                id: createSnapshotAction
                objectName: "MVACreateSnapshotAction"

                text: qsTr("&Snapshot")
                onTriggered: snapshotFileDialog.open()
            }
            Action {
                id: openProjectSettingsAction
                objectName: "MVAOpenProjectSettingsAction"

                text: qsTr("Pro&ject Settings")
                onTriggered: projectSettingsDialog.open()
            }
            Action {
                id: openSVGFolderAction
                objectName: "MVAOpenSVGFolderAction"

                text: qsTr("&Open SVG Folder")
                onTriggered: main_window.openSVGFolder()
            }
        }
        Menu {
            title: qsTr("&Help")
            Action {
                text: qsTr("&About")

                onTriggered: aboutDialog.open()
            }
        }
    }

    RowLayout {
        anchors.fill: parent

        spacing: 5

        ListView {

            id: itemView
            objectName: "MVADraggableItemListView"

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

            color: Material.color(Material.Grey)

            // TODO(codingwithmagga): Put this in a reusable component
            Item {
                id: dropAreaContainer

                property real aimedRatio: main_window.pixel_height / main_window.pixel_width

                property bool parentIsLarge: parentRatio > aimedRatio
                property real parentRatio: parent.height / parent.width
                property real availableWidth: parentIsLarge ? parent.width : height / aimedRatio

                anchors.fill: parent

                DropArea {
                    id: creationArea
                    objectName: "MVACreationArea"

                    property var abstractItem: null
                    property var abstractComponent: null

                    property double baseWidth: main_window.pixel_width
                    property double baseHeight: baseWidth * dropAreaContainer.aimedRatio
                    property double scaleFactor: dropAreaContainer.availableWidth / baseWidth

                    signal itemAdded(Item item)

                    anchors.centerIn: parent

                    width: baseWidth
                    height: baseHeight
                    scale: scaleFactor

                    clip: true

                    Drag.keys: [root.thekey]

                    onDropped: drop => {
                                   function itemClicked(itemName) {
                                       main_window.itemClickedByUser(itemName)
                                   }

                                   function animationAdded(item_name, animation_type, start_time, duration) {
                                       main_window.addAnimation(item_name,
                                                                animation_type,
                                                                start_time,
                                                                duration)
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
                                       abstractItem.animationAdded.connect(
                                           animationAdded)

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

                    // Clear current item if user clicks in an empty area
                    TapHandler {
                        onTapped: {
                            item_selection_model.setCurrentIndex(
                                        item_selection_model.model.index(-1,
                                                                         0),
                                        ItemSelectionModel.Current | ItemSelectionModel.Rows)
                        }
                    }
                }
            }
        }

        Rectangle {
            id: mObjectsListViewRect

            color: Material.color(Material.Grey)
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
                        objectName: "MVAProjectItemsTableView"

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
                        selectionModel: item_selection_model

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
                    }
                }

                TabBar {
                    id: itemBar
                    width: mObjectsListViewRect.width
                    TabButton {
                        text: qsTr("Properties")
                    }
                    TabButton {
                        text: qsTr("Animations")
                    }
                }

                StackLayout {
                    id: stackLayout

                    Layout.preferredWidth: mObjectsListViewRect.width
                    Layout.preferredHeight: mObjectsListViewRect.height / 2 - itemBar.height
                                            - horizontalHeader.implicitHeight
                    Layout.fillHeight: true

                    currentIndex: itemBar.currentIndex

                    Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                    Item {
                        id: propertiesTab

                        ColumnLayout {

                            spacing: 0

                            HorizontalHeaderView {
                                id: hHeaderPropTable
                                syncView: mPropertyTable
                                Layout.alignment: Qt.AlignTop
                                clip: true
                            }

                            ScrollView {
                                id: mPropertyTableScrollView

                                Layout.maximumWidth: mObjectsListViewRect.width
                                Layout.minimumHeight: 100
                                Layout.alignment: Qt.AlignBottom
                                Layout.fillHeight: true

                                Layout.preferredWidth: propertiesTab.width
                                Layout.preferredHeight: propertiesTab.height

                                contentWidth: mPropertyTable.width
                                contentHeight: mPropertyTable.height

                                ScrollBar.horizontal.policy: mPropertyTable.contentWidth > mPropertyTable.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                                ScrollBar.vertical.policy: mPropertyTable.contentHeight > mPropertyTable.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff

                                TableView {
                                    id: mPropertyTable

                                    alternatingRows: true
                                    clip: true

                                    columnWidthProvider: function (column) {
                                        return mPropertyTable.model ? mPropertyTable.width / mPropertyTable.model.columnCount(
                                                                          ) : 0
                                    }

                                    model: property_model

                                    selectionModel: ItemSelectionModel {
                                        id: mPropertyTableSelectionModel

                                        model: property_model
                                    }

                                    delegate: Rectangle {
                                        id: mPropertyTableDelegate

                                        required property bool editing

                                        implicitWidth: 100
                                        implicitHeight: 50

                                        color: row === mPropertyTable.currentRow ? palette.highlight : (mPropertyTable.alternatingRows && row % 2 !== 0 ? palette.alternateBase : palette.base)

                                        Label {
                                            anchors.fill: parent
                                            anchors.margins: 5
                                            verticalAlignment: TextInput.AlignVCenter

                                            text: display
                                            visible: !editing

                                            color: row === mPropertyTable.currentRow ? palette.highlightedText : palette.text
                                        }

                                        TableView.editDelegate: TextField {
                                            anchors.fill: parent

                                            text: display
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

                    Item {
                        id: animationsTab

                        ColumnLayout {

                            spacing: 0

                            HorizontalHeaderView {
                                id: headerAnimationTable
                                syncView: mAnimationTable
                                Layout.alignment: Qt.AlignTop
                                clip: true
                            }

                            ScrollView {
                                id: mAnimationTableScrollView

                                Layout.maximumWidth: mObjectsListViewRect.width
                                Layout.minimumHeight: 100
                                Layout.alignment: Qt.AlignBottom
                                Layout.fillHeight: true

                                Layout.preferredWidth: animationsTab.width
                                Layout.preferredHeight: animationsTab.height

                                contentWidth: mAnimationTable.width
                                contentHeight: mAnimationTable.height

                                ScrollBar.horizontal.policy: mAnimationTable.contentWidth > mAnimationTable.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                                ScrollBar.vertical.policy: mAnimationTable.contentHeight > mAnimationTable.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff

                                TableView {
                                    id: mAnimationTable

                                    alternatingRows: true
                                    clip: true

                                    columnWidthProvider: function (column) {
                                        return mAnimationTable.model ? mAnimationTable.width / mAnimationTable.model.columnCount(
                                                                           ) : 0
                                    }

                                    model: animation_model

                                    selectionModel: ItemSelectionModel {
                                        id: mAnimationTableSelectionModel

                                        model: animation_model
                                    }

                                    delegate: Rectangle {
                                        id: mAnimationTableDelegate

                                        implicitWidth: 100
                                        implicitHeight: 50

                                        color: row === mAnimationTable.currentRow ? palette.highlight : (mAnimationTable.alternatingRows && row % 2 !== 0 ? palette.alternateBase : palette.base)

                                        Label {
                                            anchors.fill: parent
                                            anchors.margins: 5
                                            verticalAlignment: TextInput.AlignVCenter

                                            text: display

                                            color: row === mAnimationTable.currentRow ? palette.highlightedText : palette.text
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Connections {
        target: main_window
        function onRenderingVideoFinished() {
            root.renderingVideoFinished()
        }
    }
}
