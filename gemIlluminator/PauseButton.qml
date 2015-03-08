import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Controls.Styles 1.1

Button {
    id: pauseButton
    height: 8 * Screen.pixelDensity
    width: 8 * Screen.pixelDensity
    x: root.width - width
    y: root.height - height

    property alias image: image

    style: ButtonStyle {
        background: Rectangle{
            color: "#00000000"
        }
    }

    Image {
        id: image
        height: 8 * Screen.pixelDensity
        width: 8 * Screen.pixelDensity
        source: "qrc:/data/pauseButton.png"
    }
}

