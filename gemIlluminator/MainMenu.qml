import GemIlluminator 1.0

import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.1
import QtQuick.Window 2.2

Rectangle {
    id: mainMenu
    color: "#e4eeff"

    property color backgroundBlue: "#75d5f5"
    property color lightBlue: "#33b5e5"
    property color blue: "#0099cc"
    property alias startButton: startButton

    Rectangle {
        color: backgroundBlue
        width: 70 * Screen.pixelDensity
        height: 100 + 30 * Screen.pixelDensity
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 50

        MenuButton {
            id: startButton
            label: "Start the Game!"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 70 + 20 * Screen.pixelDensity

            onClicked: {
                mainMenu.visible = false
            }
        }

        MenuButton {
            id: creditsButton
            label: "Highscore"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 30

            onClicked: {
                credits.visible = true
                mainMenu.visible = false
            }
        }

        MenuButton {
            id: optionsButton
            label: "Options"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 50 + 10 * Screen.pixelDensity

            onClicked: {
                options.visible = true
                mainMenu.visible = false
            }
        }

        MenuButton {
            id: highscoreButton
            label: "Credits..."
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 30

            onClicked: {
                credits.visible = true
                mainMenu.visible = false
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
}
