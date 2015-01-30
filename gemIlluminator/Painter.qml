import QtQuick 2.0
import GemIlluminator 1.0

Painter {
    id: painter
    visible: false

    property alias sceneId: painter.scene

    rootLightRay: LightRay {
        id: lightray
        startPosition: "0, 0, -15"
        endPosition: "0, 0, 15"
        //player: player
        //scene: painter.scene
    }
}
