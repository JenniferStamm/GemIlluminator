import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

Button {
    //width: 5 * Screen.pixelDensity
    //height: 5 * Screen.pixelDensity

    x: root.width - width
    y: root.height - height

    style: ButtonStyle {
        background: Rectangle{
            color: "#00000000"
        }
    }

    iconSource: "data/pauseButton.png"
}

