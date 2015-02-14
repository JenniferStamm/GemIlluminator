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

        property vector3d lastReading : Qt.vector3d(0, 0, 0)

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                var eulerAngle = Qt.vector3d(-rotationSensor.reading.y, rotationSensor.reading.x, 0).minus(lastReading)
                lastReading = Qt.vector3d(-rotationSensor.reading.y, rotationSensor.reading.x, 0)
                navigation.eulerRotation = eulerAngle
            }
        }
    }

    TiltSensor {
        id: tiltSensor
        dataRate: 15

        property vector3d lastReading: Qt.vector3d(0, 0, 0)

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                var eulerAngle = Qt.vector3d(-tiltSensor.reading.yRotation, tiltSensor.reading.xRotation, 0).minus(lastReading)
                lastReading = Qt.vector3d(-tiltSensor.reading.yRotation, tiltSensor.reading.xRotation, 0)
                navigation.eulerRotation = eulerAngle
            }
        }
    }
}

