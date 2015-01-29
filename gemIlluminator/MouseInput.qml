import QtQuick 2.0

MouseArea {
    id: mouseArea
    acceptedButtons: Qt.RightButton
    anchors.fill: parent
    enabled: false

    property var navigation

    property point referencePoint: Qt.point(0, 0)

    onPressed: {
        referencePoint = Qt.point(mouse.x, mouse.y)
    }

    onPositionChanged: {
        var scale = Math.min(root.width, root.height);
        var xAngle = ((mouseY - referencePoint.y) / scale) * 180 //move mouse in x direction leads to rotation to left or right
        var yAngle = ((mouseX - referencePoint.x) / scale) * 180 //move mouse in y direction leads to rotation to top or bottom
        navigation.eulerRotation = Qt.vector3d(xAngle, yAngle, 0)
        referencePoint = Qt.point(mouse.x, mouse.y)
    }
}

