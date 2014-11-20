import QtQuick 2.3
import QtQuick.Controls 1.2
import GemIlluminator 1.0
import QtSensors 5.0
import QtQml 2.2

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480

    Connections {
        target: Qt.application

        onStateChanged: {
            switch (Qt.application.state) {
            case Qt.ApplicationSuspended:
            case Qt.ApplicationHidden:
                console.log("Inactive")
                break
            case Qt.ApplicationActive:
                console.log("Active")
                break
            }
       }
    }

    Accelerometer {
        id: accel
        dataRate: 100
        active: true

        onReadingChanged: {
            var pitch = calcPitch(accel.reading.x, accel.reading.y, accel.reading.z) * .3
            var roll = calcRoll(accel.reading.x, accel.reading.y, accel.reading.z) * .3
        }
    }

    function calcPitch(x,y,z) {
        return -(Math.atan(y / Math.sqrt(x * x + z * z)) * 57.2957795);
    }
    function calcRoll(x,y,z) {
         return -(Math.atan(x / Math.sqrt(y * y + z * z)) * 57.2957795);
    }
}
