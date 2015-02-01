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
            config.loadConfig()
        }
    }
}

