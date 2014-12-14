import QtQuick 2.0

MouseArea {
    id: mouseArea
    acceptedButtons: Qt.RightButton
    anchors.fill: parent
    enabled: false

    property var navigation

    property int oldX: 0
    property int oldY: 0

    onPressed: {
        oldX = mouseX
        oldY = mouseY
    }

    onPositionChanged: {
        navigation.rotateX += ((mouseY - oldY) / root.height) * 180
        navigation.rotateY -= ((mouseX - oldX) / root.width) * 180
        oldY = mouseY
        oldX = mouseX
    }
}

