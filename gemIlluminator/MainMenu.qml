import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: mainMenu
    color: "#e5ffff"

    property alias startButton: startButton

    Button {
        id: startButton
        width: 60 * Screen.pixelDensity
        height: 15 * Screen.pixelDensity
        anchors.centerIn: mainMenu

        style: ButtonStyle {
            label: Text {
                text: "Start the Game!"
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 16
            }

            background: Rectangle{
                anchors.fill: parent
                color: control.pressed ? "#0099cc" : "#33b5e5"
            }
        }

        onClicked: {
            mainMenu.visible = false
        }

        onPressedChanged: {
            if (pressed) {
                topBorder.visible = true
                bottomBorder.visible = false

                leftBorder.color = "#33b5e5"
                rightBorder.color = "#33b5e5"
            } else {
                topBorder.visible = false
                bottomBorder.visible = true

                leftBorder.color = "#0099cc"
                rightBorder.color = "#0099cc"
            }
        }

        Rectangle {
            id: topBorder
            anchors.top: startButton.top
            color: "#33b5e5"
            height: 1 * Screen.pixelDensity
            width: startButton.width
            visible: false
        }

        Rectangle {
            id: bottomBorder
            anchors.bottom: startButton.bottom
            color: "#0099cc"
            height: 1 * Screen.pixelDensity
            width: startButton.width
            visible: true
        }

        Rectangle {
            id: leftBorder
            anchors.left: startButton.left
            color: "#0099cc"
            height: startButton.height
            width: 0.75 * Screen.pixelDensity
            visible: true
        }

        Rectangle {
            id: rightBorder
            anchors.right: startButton.right
            color: "#0099cc"
            height: startButton.height
            width: 0.75 * Screen.pixelDensity
            visible: true
        }
    }
}
