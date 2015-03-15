import QtQuick 2.0
import QtQuick.Window 2.2

/**
 * @brief Displays all people to be honored for the ongoing support of this game.
 */
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

    Text {
        font.pointSize: 18
        color: "#0099cc"
        text: "Developers"
    }

    Text {
        font.pointSize: 16
        color: "#0099cc"
        text: "Jennifer Stamm"
    }

    Text {
        font.pointSize: 16
        color: "#0099cc"
        text: "Pascal Lange"
    }

    Text {
        font.pointSize: 16
        color: "#0099cc"
        text: "Sebastian Koall"
    }
}

