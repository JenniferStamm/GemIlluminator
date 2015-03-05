import GemIlluminator 1.0

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import QtQml 2.2
import QtSensors 5.0

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    color: "red"

    Connections {
        target: Qt.application

        onStateChanged: {
            switch (Qt.application.state) {
            case Qt.ApplicationSuspended:
                painter.isAppActive = false
                console.log("Suspended")
                break
            case Qt.ApplicationHidden:
                painter.isAppActive = false
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

                painter.isAppActive = true
                console.log("Active")
                break
            case Qt.ApplicationInactive:
                painter.isAppActive = false

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
        id: inputElement
        visible: false
        focus: true

        Keys.onReleased: {
            if (((event.key === Qt.Key_Escape && Qt.platform.os !== "android") ||
                    (event.key === Qt.Key_Back && Qt.platform.os === "android")) &&
                    painter.scene !== null) {
                event.accepted = true
                mainMenu.visible = true
                Soundmanager.stopBackgroundMusic()

                painter.clearScene()
            }
        }
    }

    NavigationFile {
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

    LoadScreen {
        id: loadScreen
        anchors.fill: parent
    }

    MainMenu {
        id: mainMenu
        anchors.fill: parent

        startButton.onClicked: {
            loadScreen.visible = true

            painter.generateScene()
        }
    }

    ConfigFile {
        id: config
    }

    PainterFile {
        id: painter
    }
}
