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
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 20
        text: "Game Over"
        font.pointSize: 24
        color: "white"
    }

    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 50
        text: "Name"
        font.pointSize: 16
        color: "white"
    }


    Rectangle {
        color: "#33b5e5"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 60

        TextInput {
            id: gameOverName
            cursorVisible: false
            anchors.fill: parent
            font.pointSize: 16
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Keys.onReturnPressed: {
                submitGameOver();
            }
        }
    }

    MenuButton {
        label: "Submit Score"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 75

        onClicked: {
            submitGameOver();
        }
    }

    Text {
        anchors.top: gameOverText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Screen.pixelDensity * 2
        text: "Score: " + finalScore
        font.pointSize: 18
        color: "white"
    }

    function submitGameOver()
    {
        highscore.highscore.checkHighscore(gameOverName.text, finalScore);
        finalScore = 0;
        gameOver.visible = false;
        painter.clearScene();
        highscore.visible = true;
    }
}

