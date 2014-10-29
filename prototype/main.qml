/****************************************************************************
**
****************************************************************************/

import QtQuick 2.0
import OpenGLUnderQML 1.0

Item {
    width: 320
    height: 480

    Squircle {
        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
    }

    MultiPointTouchArea {
        anchors.fill: parent
        minimumTouchPoints: 1
        maximumTouchPoints: 5
        touchPoints: [
            TouchPoint { id: touch1 },
            TouchPoint { id: touch2 }
        ]

        onTouchUpdated: {
            if(touch1.x < parent.width / 2) {
                red.x -= 5
            }
            else {
                red.x += 5
            }

            if(touch1.y < parent.height / 2) {
                red.y -= 5
            }
            else {
                red.y += 5
            }
        }
    }

    Rectangle {
         color: "steelblue"
         width: 40
         height: 40
         x: touch2.x
         y: touch2.y
    }

    Rectangle {
        id: red
        color: "red"
        width: 120
        height: 120
        x: 0
        y: 0

        focus: true
        property bool upPressed: false
        property bool downPressed: false
        property bool leftPressed: false
        property bool rightPressed: false
        //Handling of basic key events
        Keys.onPressed: {
            if(event.key == Qt.Key_A) {
                leftPressed = true
                x -= 2
            }
            if(event.key == Qt.Key_D) {
                rightPressed = true
                x += 2
            }
            if(event.key == Qt.Key_W) {
                upPressed = true
                y -= 2
            }
            if(event.key == Qt.Key_S) {
                downPressed = true
                y += 2
            }
        }
        Keys.onReleased: {
            if(event.key == Qt.Key_A)
                leftPressed = false
            if(event.key == Qt.Key_D)
                rightPressed = false
            if(event.key == Qt.Key_W)
                upPressed = false
            if(event.key == Qt.Key_S)
                downPressed = false
        }
     }

    Text {
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "The background here is a squircle rendered with raw OpenGL using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }
}
