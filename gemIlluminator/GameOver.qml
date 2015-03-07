import QtGraphicalEffects 1.0
import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    visible: false
    anchors.fill: parent
    color: "transparent"

    property int finalScore: 0

    RadialGradient {
        anchors.fill: parent
        cached: true
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#70000000" }
            GradientStop { position: 0.325; color: "#D0000000" }
            GradientStop { position: 0.45; color: "#FF000000" }
        }
    }

    Text {
        id: gameOverText
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 20
        text: "Game Over"
        font.pointSize: 24
        color: "white"
    }

    Text {
        anchors.top: gameOverText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 2
        text: "Score: " + finalScore
        font.pointSize: 18
        color: "white"
    }
}

