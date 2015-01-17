import QtQuick 2.3
import GemIlluminator 1.0

TetrahedronGem {
    Component.onCompleted: {
        var xAngle = Math.random() * 360 - 180
        var yAngle = Math.random() * 360 - 180
        var zAngle = Math.random() * 360 - 180

        initialRotation.x = xAngle
        initialRotation.y = yAngle
        initialRotation.z = zAngle
    }
}
