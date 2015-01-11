import QtQuick 2.0
import GemIlluminator 1.0
import "gemgenerator.js" as GemGenerator

Scene {
    id: scene
    property alias cameraId: camera
    property var loadScreen: null

    camera: Camera {
        id: camera
        eye: "0, 0, 1"
        center: "0, 0, 0"
        up: "0, 1, 0"
        zNear: 0.1
        zFar: 100
        viewport.width: root.width
        viewport.height: root.height
        fovy: 60
    }

    Player {
        id: player
        velocity: 2.5
        camera: camera
    }

    rootLightRay: LightRay {
        id: lightray
        startPosition: "15, 0, 0"
        endPosition: "-15, 0, 0"
        player: player
        scene: scene
    }

    SequentialAnimation on t {
        NumberAnimation { to: 1; duration: 1000; easing.type: Easing.InQuad }
        loops: Animation.Infinite
        running: true
    }

    WorkerScript {
        id: gemGenerator
        source: "gemgenerator.js"

        onMessage: {
            var gemComponent = Qt.createComponent("gem.qml");
            var gems = messageObject.gems

            var gemsToJSON = []

            for (var i = 0; i < gems.length; i++) {
                gemsToJSON.push(gemComponent.createObject(scene,
                                                    {"id": "gem" + i.toString(),
                                                        "position.x": gems[i][0],
                                                        "position.y": gems[i][1],
                                                        "position.z": gems[i][2],
                                                    }))
            }

            console.log("Gems created: " + gems.length)

            scene.geometries = gemsToJSON
            scene.active = true

            if (loadScreen !== null) {
                loadScreen.visible = false
            }
        }
    }

    Component.onCompleted: {
        scene.active = false
        gemGenerator.sendMessage({"numGems": 600,"gemSize": 1, "rangeStart": -10, "rangeEnd": 10})
    }
}

