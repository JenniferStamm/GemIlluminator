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
    color: "red"

    property Component sceneComponent: Qt.createComponent("Scene.qml", Component.Asynchronous)
    property var scene: null

    Connections {
        target: Qt.application

        onStateChanged: {
            switch (Qt.application.state) {
            case Qt.ApplicationSuspended:
                if(scene !== null) {
                    scene.active = false
                }
                console.log("Suspended")
                break
            case Qt.ApplicationHidden:
                if(scene !== null) {
                    scene.active = false
                }
                console.log("Hidden")
                break
            case Qt.ApplicationActive:
                if(Qt.platform.os == "android") {
                    root.showFullScreen()

                    var types = QmlSensors.sensorTypes();

                    if (types.indexOf("QRotationSensor") !== -1) {
                        sensorInputs.rotationSensorId.active = true
                    }
                    else if (types.indexOf("QTiltSensor") !== -1) {
                        sensorInputs.tiltSensorId.active = true
                    }  
                }
                else {
                    mouseInput.enabled = true
                }

                if(scene !== null) {
                    scene.active = true
                }
                console.log("Active")
                break
            case Qt.ApplicationInactive:
                if(scene !== null) {
                    scene.active = false
                }

                if(Qt.platform.os === "android") {
                    root.hide()
                }

                console.log("Inactive")
                break
            }
       }
    }

    Component.onCompleted: {
        var types = QmlSensors.sensorTypes();

        if (types.indexOf("QRotationSensor") !== -1 && Qt.platform.os === "android") {
            sensorInputs.rotationSensorId.active = true
        }
        else if (types.indexOf("QTiltSensor") !== -1 && Qt.platform.os === "android") {
            sensorInputs.tiltSensorId.active = true
        }
        else {
            mouseInput.enabled = true
        }
    }

    Item {
        visible: false
        focus: true

        Keys.onReleased: {
            if (((event.key == Qt.Key_Escape && Qt.platform.os !== "android") ||
                    (event.key == Qt.Key_Back && Qt.platform.os === "android")) &&
                    scene != null) {
                event.accepted = true
                mainMenu.visible = true

                // Simple solution for stop rendering without a crash
                scene.geometries = []
                scene.delete
            }
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

    Rectangle {
        id: loadScreen
        anchors.fill: parent
        visible: false
        color: "#e5ffff"
    }

    MainMenu {
        id: mainMenu
        anchors.fill: parent

        startButton.onClicked: {
            loadScreen.visible = true
            scene = null

            scene = sceneComponent.createObject(root)
            scene.loadScreen = loadScreen
            scene.registerNavigation(navigation)
        }
    }
}
