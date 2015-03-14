import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    id: credits
    color: "#e4eeff"
    anchors.fill: parent
    visible: false

    MenuButton {
        id: back
        label: "Back"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 2 * Screen.pixelDensity

        onClicked: {
            credits.visible = false;
            mainMenu.visible = true;
        }
    }
}

