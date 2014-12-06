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

    RotationSensor {
        id: rotation
        active: true
        dataRate: 15

        onReadingChanged: {
            navigation.rotateX = rotation.reading.y * 2
            navigation.rotateY = rotation.reading.x * 2
        }
    }

    function calcPitch(x,y,z) {
        return -(Math.atan(y / Math.sqrt(x * x + z * z)) * 180 / Math.PI);
    }
    function calcRoll(x,y,z) {
         return -(Math.atan(x / Math.sqrt(y * y + z * z)) * 180 / Math.PI);
    }

    AbstractNavigation {
        id: navigation
        rotateX: 0.0
        rotateY: 0.0
        rotateZ: 0.0
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
                var x = Math.random() * 2.0 - 1.0
                var y = Math.random() * 2.0 - 1.0
                var z = Math.random() * 2.0 - 1.0
                var xAngle = Math.random() * 360 - 180
                var yAngle = Math.random() * 360 - 180
                var zAngle = Math.random() * 360 - 180
                var creationString = 'import QtQuick 2.3; import GemIlluminator 1.0; Gem {id: gem' + i
                        + '; position.x: ' + x + '; position.y: ' + y + '; position.z: ' + z
                        + '; initialRotation.x: ' + xAngle + '; initialRotation.y: ' + yAngle + '; initialRotation.z: ' + zAngle
                        + '}'
                scene.appendGeometry(Qt.createQmlObject(creationString, scene, 'gem.qml'))
            }
        }
        Component.onCompleted: {
            scene.crystalCount = 1
            scene.registerNavigation(navigation)
        }
    }

    Rectangle {
        focus: true
        Keys.onPressed: {
            if(event.key == Qt.Key_A) {
                console.log("Key pressed")
                scene.crystalCount = 1

            }
        }
    }
}
