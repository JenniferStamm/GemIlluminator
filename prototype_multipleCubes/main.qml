import QtQuick 2.3
import QtQuick.Controls 1.2
import OpenGLScene 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property real t;
    SequentialAnimation on t {
                NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
                loops: Animation.Infinite
                running: true
            }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Scene {
        id: scene
        t: root.t

        cubes: [
            Cube{
                id: cube1
                x: -1.0
                y: -1.0
                z: 0.0
                xRotation: -t * 360;
                yRotation: -t * 360;
                zRotation: 0;
            },
            Cube{
                id: cube2
                x: -1.0
                y: 1.0
                z: 0.0
                xRotation: -t * 360;
                yRotation: t * 360;
                zRotation: 0;
            },
            Cube{
                id: cube3
                x: 1.0
                y: -1.0
                z: 0.0
                xRotation: t * 360;
                yRotation: -t * 360;
                zRotation: 0;
            },
            Cube {
                id: cube4
                x: 1.0
                y: 1.0
                z: 0.0
                xRotation: t * 360;
                yRotation: t * 360;
                zRotation: 0;
            }]
    }
}
