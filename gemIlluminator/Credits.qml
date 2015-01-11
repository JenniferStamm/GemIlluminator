import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    id: credits
    color: "#e5ffff"
    anchors.fill: parent
    visible: false

    MenuButton {
        id: back
        label: "Back"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 5 * Screen.pixelDensity

        onClicked: {
            credits.visible = false
        }
    }
}
