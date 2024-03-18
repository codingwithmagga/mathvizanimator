import QtQuick
import QtTest

import cwa.mva.gui

MVACircle {
    id: test_circle

    property string circleName: "circle"
    property string clickName: ""

    width: 200
    height: 200

    SignalSpy {
        id: spy
        target: test_circle
        signalName: "clicked"
    }

    TestCase {
        id: testCase
        name: "CircleItemTests"
        when: windowShown

        function test_clicked_signal() {
            compare(spy.count, 0)
            mouseClick(test_circle)
            compare(spy.count, 1)
            compare(clickName, circleName)
        }

        function test_onpressed() {
            mousePress(test_circle)
            compare(test_circle.Drag.active, true)

            mouseRelease(test_circle)
            compare(test_circle.Drag.active, false)
        }
    }

    onClicked: name => {
                   clickName = name
               }
}
