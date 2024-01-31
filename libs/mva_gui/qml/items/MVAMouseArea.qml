
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
import QtQuick.Controls
import QtQuick.Layouts

MouseArea {
    id: mvaMouseArea

    required property var basicItem
    property var highlightRect: null
    property int highlightRectMargin: 10

    signal leftClicked
    signal animationAdded(string type, double start_time, double duration)

    acceptedButtons: Qt.LeftButton | Qt.RightButton

    onClicked: mouse => {
                   if (mouse.button === Qt.LeftButton) {
                       leftClicked()
                   } else if (mouse.button === Qt.RightButton) {
                       contextMenu.popup()
                   }
               }

    onPressed: mouse => {
                   basicItem.Drag.active = true
                   drag.target = basicItem

                   const component = Qt.createComponent(
                       "MVASurroundingRectangle.qml")
                   if (component.status === Component.Ready) {
                       highlightRect = component.createObject(parent, {})
                   } else {
                       console.log(
                           "Error creating Highlight Rectangle while moving item.")
                   }
               }

    onReleased: {
        basicItem.Drag.active = false
        drag.target = null
        highlightRect.destroy()
    }

    Menu {
        id: contextMenu
        objectName: "MVAItemContextMenu"

        MenuItem {
            objectName: "MVAItemContextMenuAddAnimation"

            text: "Add animation"

            onTriggered: {
                animationDialog.open()
            }

            function simulateClicked() {
                triggered()
            }
        }
    }

    Dialog {
        id: animationDialog
        objectName: "MVAAnimationDialog"

        anchors.centerIn: parent // TODO: change to root?
        padding: 12

        standardButtons: Dialog.Ok | Dialog.Cancel

        title: qsTr("Add animation")

        property list<double> animationData: [startTimeInputField.text, durationInputField.text]

        // Necessary for integration testing, close() is needed for macOS test, it crashes (segmentation fault) otherwise
        function simulateAccepted(start_time = "0.0", duration = "1.0") {
            startTimeInputField.text = start_time
            durationInputField.text = duration

            accepted()
            close()
        }

        ColumnLayout {
            id: mainLayout

            anchors.fill: parent
            Layout.alignment: Qt.AlignHCenter

            spacing: 40

            ColumnLayout {
                RadioButton {
                    id: fadeInRadioButton

                    checked: true
                    text: qsTr("FadeIn")
                }
                RadioButton {
                    id: fadeOutRadioButton

                    checked: false
                    text: qsTr("FadeOut")
                }
            }

            GridLayout {

                columns: 2

                rowSpacing: 20
                columnSpacing: 20

                Label {
                    text: qsTr("Start time:")

                    Layout.alignment: Qt.AlignRight

                    horizontalAlignment: Text.AlignRight
                }

                TextField {
                    id: startTimeInputField
                    objectName: "MVAStartTimeInputField"

                    text: "0.0"

                    inputMethodHints: Qt.ImhFormattedNumbersOnly
                    horizontalAlignment: TextInput.AlignRight

                    validator: DoubleValidator {
                        bottom: 0.0
                        top: 9999.99
                        notation: DoubleValidator.StandardNotation
                        decimals: 2
                        locale: "en"
                    }
                }

                Label {
                    text: qsTr("Duration:")

                    Layout.alignment: Qt.AlignRight

                    horizontalAlignment: Text.AlignRight
                }

                TextField {
                    id: durationInputField
                    objectName: "MVADurationInputField"

                    text: "1.0"
                    inputMethodHints: Qt.ImhFormattedNumbersOnly

                    horizontalAlignment: TextInput.AlignRight

                    validator: DoubleValidator {
                        bottom: 0.0
                        top: 9999.99
                        notation: DoubleValidator.StandardNotation
                        decimals: 2
                        locale: "en"
                    }
                }
            }
        }

        onAccepted: {
            var animation_type = ""

            if (fadeInRadioButton.checked) {
                animation_type = "FadeIn"
            } else if (fadeOutRadioButton.checked) {
                animation_type = "FadeOut"
            } else {
                console.log("No animation selected.")
                return
            }

            animationAdded(animation_type, startTimeInputField.text,
                           durationInputField.text)
        }
    }
}
