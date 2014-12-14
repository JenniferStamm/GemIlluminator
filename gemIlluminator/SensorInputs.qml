import QtQuick 2.0
import QtSensors 5.0

Item {
    property var navigation
    property alias rotationSensorId: rotationSensor
    property alias tiltSensorId: tiltSensor

    RotationSensor {
        id: rotationSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.rotateX = rotationSensor.reading.y * 2
                navigation.rotateY = rotationSensor.reading.x * 2
            }
        }
    }

    TiltSensor {
        id: tiltSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.rotateX = tiltSensor.reading.yRotation * 2
                navigation.rotateY = tiltSensor.reading.xRotation * 2
            }
        }
    }
}

