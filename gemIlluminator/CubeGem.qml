import QtQuick 2.3
import GemIlluminator 1.0

CubeGem {
    Component.onCompleted: {
        var xAngle = Math.random() * 360 - 180
        var yAngle = Math.random() * 360 - 180
        var zAngle = Math.random() * 360 - 180
        setRotationFromEuler(Qt.vector3d(xAngle, yAngle, zAngle))
    }
}
