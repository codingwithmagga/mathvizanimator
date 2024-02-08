import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

Dialog {
    id: createText

    anchors.centerIn: Overlay.overlay
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    title: "Text Editor"

    width: 400
    height: 200

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        textItem.latexSource = latexTextArea.text
        root.userChangedProperty(textItem.name, "latexSource",
                                 latexTextArea.text)
        createText.close()
    }
    onRejected: createText.close()

    ScrollView {
        id: view

        anchors.fill: parent

        clip: true
        focus: true

        TextArea {
            id: latexTextArea
            focus: true

            placeholderText: qsTr("Enter LaTeX text")

            text: ""

            cursorPosition: 0
            cursorVisible: true
        }
    }
}
