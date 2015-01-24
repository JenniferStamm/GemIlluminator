import QtQuick 2.0
import QtSensors 5.0

Item {
    visible: false

    property var navigation
    property alias rotationSensorId: rotationSensor
    property alias tiltSensorId: tiltSensor

    RotationSensor {
        id: rotationSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.rotateX = fromAxisAndAngle(1, 0, 0, rotationSensor.reading.x * 2)
                navigation.rotateY = fromAxisAndAngle(0, 1, 0, rotationSensor.reading.y * 2)
            }
        }
    }

    TiltSensor {
        id: tiltSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.rotateX = fromAxisAndAngle(1, 0, 0, tiltSensor.reading.xRotation * 2)
                navigation.rotateY = fromAxisAndAngle(0, 1, 0, tiltSensor.reading.yRotation * 2)
            }
        }
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

