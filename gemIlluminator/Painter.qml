import QtQuick 2.0
import GemIlluminator 1.0

Painter {
    id: painter
    visible: false
    scene: null

    property Component sceneComponent: Qt.createComponent("Scene.qml", Component.Asynchronous)

    SequentialAnimation on t {
        NumberAnimation { to: 1; duration: 1000; easing.type: Easing.InQuad }
        loops: Animation.Infinite
        running: true
    }

    function generateScene()
    {
        scene = sceneComponent.createObject(root)
        scene.loadScreen = loadScreen
        scene.registerNavigation(navigation)
    }

    function clearScene()
    {
        // Simple solution for stop rendering without a crash
        scene.geometries = []
        scene.destroy()
        scene = null
    }
}
