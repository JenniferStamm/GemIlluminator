import QtQuick 2.3
import QtQuick.Controls 1.2
import GemIlluminator 1.0
import QtSensors 5.0
import QtQml 2.2
import QtQuick.Window 2.1
import "gemgenerator.js" as GemGenerator

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    /*
    Component.onCompleted: {
        var types = QmlSensors.sensorTypes();

        if (types.indexOf("QRotationSensor") !== -1 && Qt.platform.os == "android") {
            rotationSensor.active = true
        }
        else if (types.indexOf("QTiltSensor") !== -1 && Qt.platform.os == "android") {
            tiltSensor.active = true
        }
        else {
            mouseArea.enabled = true
        }
    }

    RotationSensor {
        id: rotationSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.rotateX = rotationSensor.reading.y * 2
                navigation.rotateY = rotationSensor.reading.x * 2
            }
        }
    }

    TiltSensor {
        id: tiltSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.rotateX = tiltSensor.reading.yRotation * 2
                navigation.rotateY = tiltSensor.reading.xRotation * 2
            }
        }
    }

    MouseArea {
        id: mouseArea
        acceptedButtons: Qt.RightButton
        anchors.fill: parent
        enabled: false

        property int oldX: 0
        property int oldY: 0

        onPressed: {
            oldX = mouseX
            oldY = mouseY
        }

        onPositionChanged: {
            navigation.rotateX += ((mouseY - oldY) / root.height) * 180
            navigation.rotateY -= ((mouseX - oldX) / root.width) * 180
            oldY = mouseY
            oldX = mouseX
        }
    }
    */
    /*
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

                    var types = QmlSensors.sensorTypes();

                    if (types.indexOf("QRotationSensor") !== -1 && Qt.platform.os == "android") {
                        rotationSensor.active = true
                    }
                    else if (types.indexOf("QTiltSensor") !== -1 && Qt.platform.os == "android") {
                        tiltSensor.active = true
                    }
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
    */
    function calcPitch(x,y,z) {
        return -(Math.atan(y / Math.sqrt(x * x + z * z)) * 180 / Math.PI);
    }
    function calcRoll(x,y,z) {
         return -(Math.atan(x / Math.sqrt(y * y + z * z)) * 180 / Math.PI);
    }

    Navigation {
        id: navigation
        rotateX: 0.0
        rotateY: 0.0
        rotateZ: 0.0
    }

    Scene {
        id: scene
        camera: Camera {
            id: camera
            eye: "0, 0, 1"
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
            var gemComponent = Qt.createComponent("gem.qml");
            var gems = GemGenerator.generateGems(60, 0.5, -5, 5)

            for (var i = 0; i < gems.length; i++) {
                console.log("New gem" + i)

                scene.appendGeometry(gemComponent.createObject(scene,
                                                               {"id": "gem" + i.toString(),
                                                                   "position.x": gems[i][0],
                                                                   "position.y": gems[i][1],
                                                                   "position.z": gems[i][2],
                                                               }))
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
            if (event.key == Qt.Key_W) {
                camera.eye.z -= 0.1
            }

            if (event.key == Qt.Key_S) {
                camera.eye.z += 0.1
            }

            if (event.key == Qt.Key_Right) {
                camera.eye.x += 0.1
            }
            if (event.key == Qt.Key_Left) {
                camera.eye.x -= 0.1
            }
            if (event.key == Qt.Key_Up) {
                camera.eye.y += 0.1
            }
            if (event.key == Qt.Key_Down) {
                camera.eye.y -= 0.1
            }
        }
    }
}
