import QtQuick 2.3
import QtQuick.Controls 1.2
import GemIlluminator 1.0
import QtSensors 5.0
import QtQml 2.2
import QtQuick.Window 2.1

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480

    Connections {
        target: Qt.application

        onStateChanged: {
            switch (Qt.application.state) {
            case Qt.ApplicationSuspended:
                scene.active = false
                console.log("Suspended")
                break
            case Qt.ApplicationHidden:
                scene.active = false
                console.log("Hidden")
                break
            case Qt.ApplicationActive:
                if(Qt.platform.os == "android") {
                    root.showFullScreen()
                }

                scene.active = true
                console.log("Active")
                break
            case Qt.ApplicationInactive:
                scene.active = false

                if(Qt.platform.os == "android") {
                    root.hide()
                }

                console.log("Inactive")
                break
            }
       }
    }

    Accelerometer {
        id: accel
        dataRate: 100
        active: true

        onReadingChanged: {
            var pitch = calcPitch(accel.reading.x, accel.reading.y, accel.reading.z) * .3
            var roll = calcRoll(accel.reading.x, accel.reading.y, accel.reading.z) * .3
        }
    }

    function calcPitch(x,y,z) {
        return -(Math.atan(y / Math.sqrt(x * x + z * z)) * 57.2957795);
    }
    function calcRoll(x,y,z) {
         return -(Math.atan(x / Math.sqrt(y * y + z * z)) * 57.2957795);
    }

    Scene {
        id: scene
        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 1000; easing.type: Easing.InQuad }
            loops: Animation.Infinite
            running: true
        }
        property int crystalCount
        onCrystalCountChanged: {
            for (var i = 0; i < crystalCount; i++) {
                console.log("New gem" + i)
                var x = Math.random() * 2.0 - 1.0
                var y = Math.random() * 2.0 - 1.0
                var z = Math.random() * 2.0 - 1.0
                var xAngle = Math.random() * 360 - 180
                var yAngle = Math.random() * 360 - 180
                var zAngle = Math.random() * 360 - 180
                var creationString = 'import QtQuick 2.3; import GemIlluminator 1.0; Gem {id: gem' + i
                        + '; position.x: ' + x + '; position.y: ' + y + '; position.z: ' + z
                        + '; rotation.x: ' + xAngle + '; rotation.y: ' + yAngle + '; rotation.z: ' + zAngle
                        + '}'
                scene.appendGeometry(Qt.createQmlObject(creationString, scene, 'gem.qml'))
            }
        }
        Component.onCompleted: {
            scene.crystalCount = 3
        }
    }

    Rectangle {
        focus: true
        Keys.onPressed: {
            if(event.key == Qt.Key_A) {
                console.log("Key pressed")
                scene.crystalCount = 3

            }
        }
    }
}
