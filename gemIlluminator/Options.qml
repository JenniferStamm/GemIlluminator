import GemIlluminator 1.0
import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3

Rectangle {
    id: options
    color: "#e5ffff"
    anchors.fill: parent
    visible: false

    property int curEnvMapIndex: -1

    MenuButton {
        id: back
        label: "Save"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 2 * Screen.pixelDensity

        onClicked: {
            options.visible = false
            config.numGems = numGemsInput.text
            config.smoothnessFactor = smoothnessInput.value
            config.saveConfig()
            painter.reloadEnvMap()
            inputElement.focus = true
        }
    }

    Button {
        x: envButton.x - 12.5 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        width: 7.5 * Screen.pixelDensity
        anchors.bottomMargin: 26 * Screen.pixelDensity
        anchors.bottom: parent.bottom

        Image {
            height: 10 * Screen.pixelDensity
            width: 7.5 * Screen.pixelDensity
            source: "qrc:/data/arrow-left.png"
        }

        onClicked: {
            if (curEnvMapIndex > 0) {
                curEnvMapIndex--
                Config.envMap = config.availableEnvMaps[curEnvMapIndex]
                envMapInput.text = Config.envMap
            }
        }
    }

    Button {
        x: envButton.x + 65 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        width: 7.5 * Screen.pixelDensity
        anchors.bottomMargin: 26 * Screen.pixelDensity
        anchors.bottom: parent.bottom

        Image {
            height: 10 * Screen.pixelDensity
            width: 7.5 * Screen.pixelDensity
            source: "qrc:/data/arrow-right.png"
        }

        onClicked: {
            if (curEnvMapIndex < config.availableEnvMaps.length - 1) {
                curEnvMapIndex++
                Config.envMap = config.availableEnvMaps[curEnvMapIndex]
                envMapInput.text = Config.envMap
            }
        }
    }

    Rectangle {
        id: envButton
        color: "#33b5e5"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 26 * Screen.pixelDensity

        Text {
            id: envMapInput
            text: Config.envMap
            anchors.fill: parent
            font.pointSize: 16
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        color: "#33b5e5"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 14 * Screen.pixelDensity

        TextInput {
            id: numGemsInput
            text: config.numGems
            cursorVisible: false
            anchors.fill: parent
            font.pointSize: 16
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 38 * Screen.pixelDensity
        color: "#33b5e5"
        height: 10 * Screen.pixelDensity
        width: 60 * Screen.pixelDensity
        Text {
            text: "Control Sensitivity:"
            font.pointSize: 12
            color: "white"
            anchors.leftMargin: 1 * Screen.pixelDensity
            anchors.bottom: parent.verticalCenter
            anchors.left: parent.left
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: smoothnessInput
            value: config.smoothnessFactor
            stepSize: 0.1
            minimumValue: 0.1
            maximumValue: 1.0
            anchors.top: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            style: SliderStyle {
                groove: Rectangle {
                    color: "white"
                    implicitWidth: 55 * Screen.pixelDensity
                    implicitHeight: 1 * Screen.pixelDensity
                }
                handle: Rectangle {
                    color: "#0099cc"
                    implicitWidth: 2 * Screen.pixelDensity
                    implicitHeight: 4 * Screen.pixelDensity
                }
            }
        }

    }

    onVisibleChanged: {
        if (visible == true) {
            curEnvMapIndex = config.availableEnvMaps.indexOf(config.envMap)
        }
    }
}

