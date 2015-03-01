import QtQuick 2.0
import QtSensors 5.0

Item {
    visible: false

    property var navigation
    property alias rotationSensorId: rotationSensor
    property alias tiltSensorId: tiltSensor
    property vector3d lastReading : Qt.vector3d(0, 0, 0)

    RotationSensor {
        id: rotationSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                var currentReading = Qt.vector3d(-rotationSensor.reading.y, rotationSensor.reading.x, 0)
                navigation.eulerRotation = smoothedEulerAngle(currentReading)
            }
        }
    }

    TiltSensor {
        id: tiltSensor
        dataRate: 15

        onReadingChanged: {
            if(Qt.platform.os == "android") {
                var currentReading = Qt.vector3d(-tiltSensor.reading.yRotation, tiltSensor.reading.xRotation, 0)
                navigation.eulerRotation = smoothedEulerAngle(currentReading)
            }
        }
    }

    function smoothedEulerAngle(currentReading) {
        var currentFilteredReading = lastReading.plus((currentReading.minus(lastReading)).times(config.smoothnessFactor))
        var eulerAngle = currentFilteredReading.minus(lastReading)
        lastReading = currentFilteredReading
        return eulerAngle
    }
}

