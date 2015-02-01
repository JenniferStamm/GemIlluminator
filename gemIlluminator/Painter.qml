import QtQuick 2.0
import GemIlluminator 1.0

Painter {
    id: painter
    visible: false

    property Component sceneComponent: Qt.createComponent("Scene.qml", Component.Asynchronous)
    property var sceneTemp: null

    SequentialAnimation on t {
        NumberAnimation { to: 1; duration: 1000; easing.type: Easing.InQuad }
        loops: Animation.Infinite
        running: true
    }

    function generateScene()
    {
        sceneTemp = sceneComponent.createObject(root)
        sceneTemp.loadScreen = loadScreen
        sceneTemp.registerNavigation(navigation)
        painter.scene = sceneTemp
    }

    function clearScene()
    {
        // Simple solution for stop rendering without a crash
        painter.scene.geometries = []
        painter.scene.destroy(5)
        sceneTemp = null
        painter.scene = null
    }
}
