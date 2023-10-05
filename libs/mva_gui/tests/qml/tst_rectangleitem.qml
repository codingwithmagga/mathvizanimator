import QtQuick
import QtTest

import cwa.mva.gui

MVARectangle {
    id: test_rect

    property string rectName: "testName"
    property string clickName: ""

    width: 200
    height: 200
    item.name: rectName

    SignalSpy {
        id: spy
        target: test_rect
        signalName: "clicked"
    }

    TestCase {
        id: testCase
        name: "RectangleItemTests"
        when: windowShown

        function test_clicked_signal() {
            compare(spy.count, 0)
            mouseClick(test_rect)
            compare(spy.count, 1)
            compare(clickName, rectName)
        }

        function test_onpressed() {
            mousePress(test_rect)
            compare(test_rect.dragActive, true)

            mouseRelease(test_rect)
            compare(test_rect.dragActive, false)
        }
    }

    onClicked: name => {
                   clickName = name
               }
}
