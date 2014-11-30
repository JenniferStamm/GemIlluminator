import QtQuick 2.3
import QtQuick.Controls 1.2
import GemIlluminator 1.0
import QtSensors 5.0
import QtQml 2.2

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
                console.log("Suspended")
                break
            case Qt.ApplicationHidden:
                console.log("Hidden")
                break
            case Qt.ApplicationActive:
                console.log("Active")
                break
            case Qt.ApplicationInactive:
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
        camera: Camera {
            id: camera
            eye: "4, 2, 1"
            center: "0, 0, 0"
            up: "0, 1, 0"
            zNear: 0.1
            zFar: 100
            viewport.width: root.width
            viewport.height: root.height
            fovy: 60
        }

        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 1000; easing.type: Easing.InQuad }
            loops: Animation.Infinite
            running: true
        }
        property int crystalCount
        onCrystalCountChanged: {
            for (var i = 0; i < crystalCount; i++) {
                console.log("New gem" + i)

                scene.appendGeometry(Qt.createQmlObject('import QtQuick 2.3; import GemIlluminator 1.0; Gem {id: gem}', scene, 'gem.qml'))
            }
        }
        Component.onCompleted:
            scene.crystalCount = 3
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
