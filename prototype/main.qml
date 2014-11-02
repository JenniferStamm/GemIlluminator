/****************************************************************************
**
****************************************************************************/

import QtQuick 2.0
import GPCube 1.0
import QtSensors 5.0
import QtMultimedia 5.0

Item {
    id: mainWindow
    width: 320
    height: 480

    Cube {
        id: cube
        visible: false

        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }

    // Demonstrates the use of touch events
    MouseArea {
        anchors.fill: parent

        property int pressX
        property int pressY

        onDoubleClicked: {
            doubleTap()
        }

        onPressed: {
            if(green.visible == true) {
                pressX = mouseX
                pressY = mouseY
            }
        }

        onReleased: {
            if(green.visible == true) {
                timer.start()
                green.startX = pressX
                green.startY = pressY
                green.stopX = mouseX
                green.stopY = mouseY
            }
        }

        onPositionChanged: {
            if(red.visible == true) {
                var speed = 10
                var x = mouseX - red.x
                var y = mouseY - red.y
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
    }

    function doubleTap() {
        if(red.visible == true) {
            red.visible = false
            blue.visible = true
            accel.active = true
        }
        else if(blue.visible == true) {
            blue.visible = false
            accel.active = false
            green.visible = true
            timer.start()
        }
        else if(green.visible == true) {
            green.visible = false
            timer.stop()
            cube.visible = true
        }
        else if(cube.visible == true) {
            cube.visible = false
            red.visible = true
        }
    }

    // Demonstrates the use of the motion sensor
    Accelerometer {
        id: accel
        dataRate: 100
        active: false

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
        visible: false
    }

    // Demonstrates swipe speed motions
    Rectangle {
        id: green
        color: "green"
        width: 80
        height: 80
        visible: false

        property int startX
        property int startY
        property int stopX
        property int stopY

        Timer {
            id: timer
            interval: 10; running: false; repeat: true
            onTriggered: {
                var changeX = 20 * ((green.stopX - green.startX) / mainWindow.width)
                var changeY = 20 * ((green.stopY - green.startY) / mainWindow.height)

                if(green.x + changeX < 0) {
                    changeX = 0
                }

                if(green.y + changeY < 0) {
                    changeY = 0
                }

                if(changeX + green.x + green.width >= green.parent.width) {
                    changeX = 0
                }

                if(changeY + green.y + green.height >= green.parent.height) {
                    changeY = 0
                }

                green.x += changeX
                green.y += changeY
            }
        }

    }

    Rectangle {
        id: red
        color: "red"
        width: 80
        height: 80
        x: 0
        y: 0
        visible: true

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
}
