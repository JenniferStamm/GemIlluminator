import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQml 2.2
import QtQuick.Window 2.1
import QtSensors 5.0

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

                    var types = QmlSensors.sensorTypes();

                    if (types.indexOf("QRotationSensor") !== -1 && Qt.platform.os == "android") {
                        sensorInputs.rotationSensorId.active = true
                    }
                    else if (types.indexOf("QTiltSensor") !== -1 && Qt.platform.os == "android") {
                        sensorInputs.tiltSensorId.active = true
                    }
                    else {
                        mouseInput.enabled = true
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

    Component.onCompleted: {
        var types = QmlSensors.sensorTypes();

        if (types.indexOf("QRotationSensor") !== -1 && Qt.platform.os == "android") {
            sensorInputs.rotationSensorId.active = true
        }
        else if (types.indexOf("QTiltSensor") !== -1 && Qt.platform.os == "android") {
            sensorInputs.tiltSensorId.active = true
        }
        else {
            mouseInput.enabled = true
        }
    }

    Item {
        focus: true

        Keys.onPressed: {           
            if (event.key == Qt.Key_W) {
                scene.cameraId.eye.z -= 0.1
            }

            if (event.key == Qt.Key_S) {
                scene.cameraId.eye.z += 0.1
            }

            if (event.key == Qt.Key_Right) {
                scene.cameraId.eye.x += 0.1
            }
            if (event.key == Qt.Key_Left) {
                scene.cameraId.eye.x -= 0.1
            }
            if (event.key == Qt.Key_Up) {
                scene.cameraId.eye.y += 0.1
            }
            if (event.key == Qt.Key_Down) {
                scene.cameraId.eye.y -= 0.1
            }
        }
    }

    Scene {
        id: scene

        Component.onCompleted: {
            scene.registerNavigation(navigation)
        }
    }

    Navigation {
        id: navigation
    }

    SensorInputs {
        id: sensorInputs
        navigation: navigation
    }

    MouseInput {
        id: mouseInput
        navigation: navigation
    }
}
