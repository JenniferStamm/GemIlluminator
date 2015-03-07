import QtQuick 2.3
import GemIlluminator 1.0

CubeGem {
    property real xAngle
    property real yAngle
    property real zAngle

    Component.onCompleted: {
        setRotationFromEuler(Qt.vector3d(xAngle, yAngle, zAngle))
    }
}
