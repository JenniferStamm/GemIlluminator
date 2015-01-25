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
                navigation.setRotationFromEuler(Qt.vector3d(rotationSensor.reading.y * 2, rotationSensor.reading.x * 2, 0))
            }
        }
    }

    TiltSensor {
        id: tiltSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                navigation.setRotationFromEuler(Qt.vector3d(tiltSensor.reading.y * 2, tiltSensor.reading.x * 2, 0))
            }
        }
    }
}

