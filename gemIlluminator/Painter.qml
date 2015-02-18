import QtQuick 2.0
import GemIlluminator 1.0

Painter {
    id: painter
    visible: false
    scene: null
    envMapPrefix: config.envMap

    property Component sceneComponent: Qt.createComponent("Scene.qml", Component.Asynchronous)

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
}
