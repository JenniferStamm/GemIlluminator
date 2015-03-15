import GemIlluminator 1.0
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.2

/**
 * @brief Provides an input for the user so he is able to enter a seed to generate the scene.
 */
Rectangle {
    id: seedInput
    visible: false
    anchors.fill: parent
    color: "#e4eeff"

    MenuButton {
        label: "Generate Scene"
        x: parent.width / 2 - width / 2
        y: parent.height / 2 + 10 * Screen.pixelDensity

        onClicked: {
            seedInput.visible = false;
            loadScreen.visible = true;
            inputElement.focus = true;

            Soundmanager.playBackgroundMusic();
            painter.seed = seedString.text;
            painter.generateScene();
        }
    }

    Text {
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - 15 * Screen.pixelDensity
        font.pointSize: 16
        color: "#0099cc"
        text: "Seed:"
    }

    Rectangle {
        color: "#33b5e5"
        width: 60 * Screen.pixelDensity
        height: 10 * Screen.pixelDensity
        anchors.verticalCenter:  parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        TextInput {
            id: seedString
            cursorVisible: false
            anchors.fill: parent
            font.pointSize: 16
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
}

