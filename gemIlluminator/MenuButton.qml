import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

/**
 * @brief A generic button for any menu interaction.
 */
Button {
    width: 60 * Screen.pixelDensity
    height: 10 * Screen.pixelDensity

    property color lightBlue: "#33b5e5"
    property color blue: "#0099cc"
    property alias label: buttonLabel.text

    style: ButtonStyle {
        background: Rectangle{
            anchors.fill: parent
            color: lightBlue
        }
    }

    Text {
        id: buttonLabel
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 16
        anchors.fill: parent
    }

    onPressedChanged: {
        if (pressed) {
            bottomBorder.visible = false
            leftBorder.visible = false
            rightBorder.visible = false
        } else {
            bottomBorder.visible = true
            leftBorder.visible = true
            rightBorder.visible = true
        }
    }

    Rectangle {
        id: bottomBorder
        anchors.bottom: parent.bottom
        color: blue
        height: 2
        width: parent.width
        visible: true
    }

    Rectangle {
        id: leftBorder
        anchors.left: parent.left
        color: blue
        height: parent.height
        width: 3
        visible: true
    }

    Rectangle {
        id: rightBorder
        anchors.right: parent.right
        color: blue
        height: parent.height
        width: 3
        visible: true
    }
}
