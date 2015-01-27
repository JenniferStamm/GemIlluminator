import QtQuick 2.0

MouseArea {
    id: mouseArea
    acceptedButtons: Qt.RightButton
    anchors.fill: parent
    enabled: false

    property var navigation

    property real xAngle: 0
    property real yAngle: 0

    property vector3d mouseVector

    onPositionChanged: {
        var hackie = Math.min(root.width, root.height);
        mouseVector = Qt.vector3d(mouseX, -mouseY, 0)
        var xAngle = (mouseY / hackie) * 360 - 180 //move mouse in x direction leads to rotation to left or right
        var yAngle = (mouseX / hackie) * 360 - 180  //move mouse in y direction leads to rotation to top or bottom
        navigation.eulerRotation = Qt.vector3d(xAngle, yAngle, 0)
    }
}

