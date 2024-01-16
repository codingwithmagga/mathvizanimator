
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
import QtQuick.Layouts
import QtQuick.Controls

import cwa.mva.gui

MVABasicItem {
    id: rootMVAText

    property alias item: textItem

    TextItem {
        id: textItem

        name: "text"

        anchors.fill: parent

        MVAMouseArea {
            anchors.fill: parent
            basicItem: rootMVAText

            onLeftClicked: {
                rootMVAText.clicked(textItem.name)
            }

            // TODO Use basicItem of MVAMouseArea for this an move this to MVAMouseArea.qml, see Issue #83
            onAnimationAdded: (type, start_time, duration) => {
                                  rootMVAText.animationAdded(textItem.name,
                                                             type, start_time,
                                                             duration)
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

        background: Rectangle {
            color: palette.window
        }

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
                    color: palette.base
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
