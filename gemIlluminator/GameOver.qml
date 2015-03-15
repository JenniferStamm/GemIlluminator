import QtGraphicalEffects 1.0
import QtQuick 2.0
import QtQuick.Window 2.2

Rectangle {
    id: gameOver
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
        y: parent.height / 2 - parent.height / 3
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Game Over"
        font.pointSize: 24
        color: "white"
    }

    Text {
        anchors.top: gameOverText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 2 * Screen.pixelDensity
        text: "Score: " + finalScore
        font.pointSize: 18
        color: "white"
    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: "Name"
        font.pointSize: 16
        color: "white"
    }


    Rectangle {
        y: parent.height / 2 + 5 * Screen.pixelDensity
        color: "#33b5e5"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.horizontalCenter: parent.horizontalCenter

        TextInput {
            id: gameOverName
            cursorVisible: false
            anchors.fill: parent
            font.pointSize: 16
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    MenuButton {
        label: "Submit Score"
        y: parent.height / 2 + 18 * Screen.pixelDensity
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.horizontalCenter: parent.horizontalCenter

        onClicked: {
            highscore.highscore.checkHighscore(gameOverName.text, finalScore);
            finalScore = 0;
            gameOver.visible = false;
            painter.clearScene();
            highscore.visible = true;
        }
    }
}

