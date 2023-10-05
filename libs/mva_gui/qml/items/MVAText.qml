import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import cwa.mva.gui

MVABasicItem {
    id: rootMVAText

    property alias item: textItem

    TextItem {
        id: textItem

        name: "text"
        color: "black"

        height: parent.height
        width: parent.width

        MVAMouseArea {
            anchors.fill: parent

            onClicked: {
                rootMVAText.clicked(textItem.name)
            }
        }
    }

    Component.onCompleted: {
        if (init) {
            createText.open()
        }
    }

    Popup {
        id: createText

        anchors.centerIn: Overlay.overlay
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        width: 400

        ColumnLayout {
            anchors.fill: parent
            spacing: 20

            ScrollView {
                id: view

                Layout.preferredHeight: 200
                Layout.fillHeight: true
                Layout.fillWidth: true

                anchors.margins: 60

                background: Rectangle {
                    color: "black"
                }

                focus: true

                TextArea {
                    id: latexTextArea
                    focus: true

                    text: ""
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight

                spacing: 10

                Button {
                    text: qsTr("Close")

                    onClicked: createText.close()
                }

                Button {
                    text: qsTr("Accept")

                    onClicked: {
                        textItem.latexSource = latexTextArea.text
                        createText.close()
                    }
                }
            }
        }
    }
}
