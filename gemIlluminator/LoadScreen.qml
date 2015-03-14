import QtQuick 2.3
import QtQuick.Controls 1.2

Rectangle {
    visible: false
    color: "#e4eeff"

    property real currentProgress: 0.0

    Rectangle {
        color: "#75d5f5"
        x: parent.width * 0.05
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width * 0.9 * parent.currentProgress
        height: parent.height * 0.1
    }

    Rectangle {
        color: "transparent"
        x: parent.width * 0.05
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width * 0.9
        height: parent.height * 0.1
        border.color: "#33b5e5"
        border.width: 2
    }
}

