import QtQuick 2.0

MouseArea {
    id: mouseArea
    acceptedButtons: Qt.RightButton
    anchors.fill: parent
    enabled: false

    property var navigation

    property int oldX: root.width / 2
    property int oldY: root.height / 2

    property real xAngle: 0
    property real yAngle: 0

    property vector3d mouseVector

    onPressed: {
//        oldX = mouseX
//        oldY = mouseY
    }

    onPositionChanged: {
        var hackie = Math.min(root.width, root.height);
        mouseVector = Qt.vector3d(mouseX, -mouseY, 0)
        var testX = (mouseX / hackie) * 360 - 180
        var testY = (-mouseY / hackie) * 360 - 180
        var dotX = mouseVector.x / mouseVector.length()
        var dotY = mouseVector.y / mouseVector.length()
        var xAngle = Math.acos(dotX) * 180.0 / (Math.PI)
        var yAngle = Math.acos(dotY) * 180.0 / (Math.PI)
        console.log("mouseX: " + mouseVector.x + ", xAngle: " + xAngle)
        console.log("mouseY: " + mouseVector.y + ", yAngle: " + yAngle)
        navigation.rotateX = fromAxisAndAngle(1, 0, 0, testX)
        navigation.rotateY = fromAxisAndAngle(0, 1, 0, testY)
    }

    function fromAxisAndAngle(x, y, z, angle)
    {
        var a = (angle / 2.0) * Math.PI / 180.0
        var s = Math.sin(a)
        var c = Math.cos(a)
        var ax = Qt.vector3d(x, y, z)
        ax.normalized()
        var result = Qt.quaternion(c, ax.x * s, ax.y * s, ax.z * s)
        return result
    }
}

