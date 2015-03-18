import GemIlluminator 1.0
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.2

/**
 * @brief QML object of the scene. This object is created for every game.
 */
Rectangle {
    id: highscoreView
    anchors.fill: parent
    visible: false
    color: "#e4eeff"

    property alias menuButton: menuButton
    property alias highscore: highscore

    Highscore {
        id: highscore
        source: "highscore.dat"

        function checkHighscore(name, score)
        {
            var i, numScores
            var highscoreData = highscore.read()

            highscoreData = highscoreData.split("-;")

            if (Qt.platform.os  === "android") {
                numScores = 5
            } else {
                numScores = 10
            }

            for (i = 0; i < numScores; i++) {
                if (score > parseInt(highscoreData[i * 2 + 1])) {
                    highscoreData.splice(i * 2, 0, score);
                    highscoreData.splice(i * 2, 0, name);
                    highscoreData.pop();
                    highscoreData.pop();
                    highscore.write(highscoreData.join("-;\n"));
                    break;
                }
            }
        }
    }

    MenuButton {
        id: menuButton
        label: "To Mainmenu"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 2 * Screen.pixelDensity

        onClicked: {
            highscoreView.visible = false
            mainMenu.visible = true
        }
    }

    Rectangle {
        id: scoreContainer
        anchors.fill: parent
        color: "#00000000"
    }

    onVisibleChanged: {
        if (visible) {
            var i, name, score, numScores;
            var highscoreData = highscore.read();

            highscoreData = highscoreData.split("-;");

            for (i = 0; i < scoreContainer.children.length; ++i) {
                scoreContainer.children[i].destroy();
            }

            if (Qt.platform.os  === "android") {
                numScores = 5
            } else {
                numScores = 10
            }

            for (i = 0; i < numScores; i++) {
                score = highscoreData.pop();
                name = highscoreData.pop();
                Qt.createQmlObject(getHighscoreNameString(name, (numScores - 1) - i),
                     scoreContainer, "dynamicSnippet" + i + "_1");
                Qt.createQmlObject(getHighscoreScoreString(score, (numScores - 1) - i),
                     scoreContainer, "dynamicSnippet" + i + "_2");
            }
        }
    }

    function getHighscoreNameString(name, i)
    {
        var nameString = 'import QtQuick 2.0;';

        nameString += 'import QtQuick.Window 2.2;';
        nameString += 'Text {';
        nameString += 'color: "#0099cc";';
        nameString += 'width: 20;';
        nameString += 'height: 20;';
        nameString += 'font.pointSize: 16;';
        nameString += 'text: "' + name + '";';

        if (Qt.platform.os  === "android") {
            nameString += 'x: (parent.width / 2) + 30 * Screen.pixelDensity;';
        } else {
            nameString += 'x: (parent.width / 2) - 120;';
        }

        nameString += 'anchors.top: parent.top;';
        nameString += 'anchors.topMargin: 60 + 5 * ' + i + ' * Screen.pixelDensity;';
        nameString += '}';

        return nameString;
    }

    function getHighscoreScoreString(score, i)
    {
        var scoreString = 'import QtQuick 2.0;';

        scoreString += 'import QtQuick.Window 2.2;';
        scoreString += 'Text {';
        scoreString += 'color: "#0099cc";';
        scoreString += 'width: 20;';
        scoreString += 'height: 20;';
        scoreString += 'font.pointSize: 16;';
        scoreString += 'text: "' + score + '";';

        if (Qt.platform.os  === "android") {
            scoreString += 'x: (parent.width / 2) - 25 * Screen.pixelDensity;';
        } else {
            scoreString += 'x: (parent.width / 2) + 85;';
        }
        scoreString += 'anchors.top: parent.top;';
        scoreString += 'anchors.topMargin: 60 + 5 * ' + i + ' * Screen.pixelDensity;';
        scoreString += '}';

        return scoreString;
    }
}

