import QtQuick 2.0
import GemIlluminator 1.0

Painter {
    id: painter
    visible: false
    scene: null

    property Component sceneComponent: Qt.createComponent("Scene.qml", Component.Asynchronous)
    property string seed: ""

    function generateScene()
    {
        scene = sceneComponent.createObject(root)
        scene.loadScreen = loadScreen
        scene.registerNavigation(navigation)
    }

    function clearScene()
    {
        // Simple solution for stop rendering without a crash
        active = false
        scene.geometries = []
        scene.destroy()
        scene = null
    }

    onIsAppActiveChanged: {
        if (scene !== null) {
            if (isAppActive) {
                scene.timerId.start()
            } else {
                scene.timerId.stop()
            }
        }
    }
}
