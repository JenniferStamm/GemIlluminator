import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 1.2

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
        anchors.bottomMargin: 5 * Screen.pixelDensity

        onClicked: {
            options.visible = false
            config.numGems = numGemsInput.text
            config.saveConfig()
            inputElement.focus = true
        }
    }

    Button {
        x: envButton.x - 12.5 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        width: 7.5 * Screen.pixelDensity
        anchors.bottomMargin: 35 * Screen.pixelDensity
        anchors.bottom: parent.bottom

        Image {
            height: 10 * Screen.pixelDensity
            width: 7.5 * Screen.pixelDensity
            source: "qrc:/data/arrow-left.png"
        }

        onClicked: {
            if (curEnvMapIndex > 0) {
                curEnvMapIndex--
                config.envMap = config.availableEnvMaps[curEnvMapIndex]
                envMapInput.text = config.envMap
            }
        }
    }

    Button {
        x: envButton.x + 65 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        width: 7.5 * Screen.pixelDensity
        anchors.bottomMargin: 35 * Screen.pixelDensity
        anchors.bottom: parent.bottom

        Image {
            height: 10 * Screen.pixelDensity
            width: 7.5 * Screen.pixelDensity
            source: "qrc:/data/arrow-right.png"
        }

        onClicked: {
            if (curEnvMapIndex < config.availableEnvMaps.length - 1) {
                curEnvMapIndex++
                config.envMap = config.availableEnvMaps[curEnvMapIndex]
                envMapInput.text = config.envMap
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
        anchors.bottomMargin: 35 * Screen.pixelDensity

        Text {
            id: envMapInput
            text: config.envMap
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
        anchors.bottomMargin: 20 * Screen.pixelDensity

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

    onVisibleChanged: {
        if (visible == true) {
            curEnvMapIndex = config.availableEnvMaps.indexOf(config.envMap)
        }
    }
}

