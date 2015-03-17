import QtQuick 2.4
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

    Column {
        y: 5 * Screen.pixelDensity
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2

        ParentCenteredText {
            font.pointSize: 19
            text: "Developers"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Jennifer Stamm"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Pascal Lange"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Sebastian Koall\n"
        }

        ParentCenteredText {
            font.pointSize: 19
            text: "Mentors"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Daniel Limberger"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Martin Reiter\n"
        }

        ParentCenteredText {
            font.pointSize: 19
            text: "Special Thanks"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Philipp Fischbeck"
        }

        ParentCenteredText {
            font.pointSize: 16
            text: "Daniel Beard"
        }

        Text {
            font.pointSize: 16
            text: "www.freesfx.co.uk"
            color: "#0099cc"
            horizontalAlignment: Text.AlignHCenter
        }
    }
}

