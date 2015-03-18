import QtQuick 2.0
import GemIlluminator 1.0

/**
 * @brief QML object of the painter. The file is named PainterView.qml to prevent name collisions in the main.qml.
 */
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
        isGameActive = false
        scene.geometries = []
        scene.destroy()
        scene = null
    }
}
