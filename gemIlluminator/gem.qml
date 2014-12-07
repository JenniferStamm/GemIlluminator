import QtQuick 2.3
import GemIlluminator 1.0

Gem {
    Component.onCompleted: {
        var x = Math.random() * 2.0 - 1.0
        var y = Math.random() * 2.0 - 1.0
        var z = Math.random() * 2.0 - 1.0
        var xAngle = Math.random() * 360 - 180
        var yAngle = Math.random() * 360 - 180
        var zAngle = Math.random() * 360 - 180

        position.x = x
        position.y = y
        position.z = z
        initialRotation.x = xAngle
        initialRotation.y = yAngle
        initialRotation.z = zAngle
    }
}
