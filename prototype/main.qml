/****************************************************************************
**
****************************************************************************/

import QtQuick 2.0
import OpenGLUnderQML 1.0
import QtSensors 5.0

Item {
    id: mainWindow
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

    // Demonstrates the use of touch events
    MultiPointTouchArea {
        anchors.fill: parent
        minimumTouchPoints: 1
        maximumTouchPoints: 5
        touchPoints: [
            TouchPoint { id: touch1 },
            TouchPoint { id: touch2 }
        ]

        onTouchUpdated: {
            var speed = 10
            var x = touch1.x - red.x
            var y = touch1.y - red.y
            var diagonal = Math.sqrt(x * x + y * y)
            var normX = (x / diagonal) * speed
            var normY = (y / diagonal) * speed

            if(red.x + normX < 0) {
                normX = 0
            }

            if(normX + red.x + red.width >= parent.width) {
                red.x = parent.width - red.width
            }

            if(red.y + normY < 0) {
                normY = 0
            }

            if(normY + red.y + red.height >= parent.height) {
                red.y = parent.height - red.height
            }

            red.x += normX
            red.y += normY
        }
    }

    Rectangle {
        id: red
        color: "red"
        width: 80
        height: 80
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

    // Demonstrates the use of the motion sensor
    Accelerometer {
        id: accel
        dataRate: 100
        active:true

        onReadingChanged: {
            // Depending on the screen orientation x and y need to be switched
            // (currently configurated for landscape on S4)
            var newX = (blue.x - calcPitch(accel.reading.x, accel.reading.y, accel.reading.z) * .3)
            var newY = (blue.y - calcRoll(accel.reading.x, accel.reading.y, accel.reading.z) * .3)

            if(isNaN(newX) || isNaN(newY)) {
                return;
            }

            if(newX < 0) {
                newX = 0
            }

            if(newX > mainWindow.width - blue.width) {
                newX = mainWindow.width - blue.width
            }

            if(newY < 0) {
                newY = 0
            }

            if(newY > mainWindow.height - blue.height) {
                newY = mainWindow.height - blue.height
            }

            blue.x = newX
            blue.y = newY
        }
    }

    function calcPitch(x,y,z) {
        return -(Math.atan(y / Math.sqrt(x * x + z * z)) * 57.2957795);
    }
    function calcRoll(x,y,z) {
         return -(Math.atan(x / Math.sqrt(y * y + z * z)) * 57.2957795);
    }

    Rectangle {
        id: blue
        color: "steelblue"
        width: 80
        height: 80
    }

    // etc.
    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 10
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
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
