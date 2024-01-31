import QtQuick
import QtTest

import cwa.mva.gui

MVAText {
    id: test_text

    property string textName: "text"
    property string clickName: ""

    width: 200
    height: 200

    SignalSpy {
        id: spy
        target: test_text
        signalName: "clicked"
    }

    TestCase {
        id: testCase
        name: "TextItemTests"
        when: windowShown

        function test_clicked_signal() {
            compare(spy.count, 0)
            mouseClick(test_text)
            compare(spy.count, 1)
            compare(clickName, textName)
        }

        function test_onpressed() {
            mousePress(test_text)
            compare(test_text.Drag.active, true)

            mouseRelease(test_text)
            compare(test_text.Drag.active, false)
        }
    }

    onClicked: name => {
                   clickName = name
               }
}
