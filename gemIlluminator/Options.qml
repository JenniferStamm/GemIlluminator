import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    id: options
    color: "#e5ffff"
    anchors.fill: parent
    visible: false

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

    Image {
        x: envButton.x - 12.5 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        width: 7.5 * Screen.pixelDensity
        source: "qrc:/data/arrow-left.png"
        anchors.bottomMargin: 35 * Screen.pixelDensity
        anchors.bottom: parent.bottom
     }

    Image {
        x: envButton.x + 65 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        width: 7.5 * Screen.pixelDensity
        source: "qrc:/data/arrow-right.png"
        anchors.bottomMargin: 35 * Screen.pixelDensity
        anchors.bottom: parent.bottom
     }

    Rectangle {
        id: envButton
        color: "#33b5e5"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 35 * Screen.pixelDensity
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
}

