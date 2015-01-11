import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: mainMenu
    color: "#e5ffff"

    property color backgroundBlue: "#75d5f5"
    property color lightBlue: "#33b5e5"
    property color blue: "#0099cc"
    property alias startButton: startButton

    Rectangle {
        color: backgroundBlue
        width: 70 * Screen.pixelDensity
        height: 80 + 20 * Screen.pixelDensity
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 50

        MenuButton {
            id: startButton
            label: "Start the Game!"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 50 + 10 * Screen.pixelDensity

            onClicked: {
                mainMenu.visible = false
            }
        }

        MenuButton {
            id: creditsButton
            label: "Credits..."
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 30

            onClicked: {
                credits.visible = true
            }
        }

        Rectangle {
            id: bottomBorder
            anchors.bottom: parent.bottom
            color: lightBlue
            height: 0.75 * Screen.pixelDensity
            width: parent.width
        }

        Rectangle {
            id: leftBorder
            anchors.left: parent.left
            color: lightBlue
            height: parent.height
            width: 0.5 * Screen.pixelDensity
        }

        Rectangle {
            id: rightBorder
            anchors.right: parent.right
            color: lightBlue
            height: parent.height
            width: 0.5 * Screen.pixelDensity
        }
    }

    Credits {
        id: credits

    }
}
