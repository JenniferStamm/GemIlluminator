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

    previewCamera: Camera {
        id: previewCamera
        eye: "0, 0, 1"
        center: "0, 0, 0"
        up: "0, 1, 0"
        zNear: 0.1
        zFar: 100
        viewport.width: root.width / config.viewportRatio
        viewport.height: root.height / config.viewportRatio
        fovy: 60
    }

    Player {
        id: player
        velocity: 2.5
        camera: camera
    }


    rootLightRay: LightRay {
        id: lightray
        startPosition: "0, 0, -15"
        endPosition: "0, 0, 15"
        player: player
        scene: scene
    }

    WorkerScript {
        id: gemGenerator
        source: "gemgenerator.js"

        onMessage: {
            if(messageObject.gems) {
                var gems = messageObject.gems
                var gemTypes = initGemTypes()

                var gemsToJSON = []
                var curGemType = null

                for (var i = 0; i < gems.length; i++) {
                    curGemType = gems[i][4].toString()
                    gemsToJSON.push(gemTypes[curGemType].createObject(scene,
                                                        {"id": "gem" + i.toString(),
                                                            "position.x": gems[i][0],
                                                            "position.y": gems[i][1],
                                                            "position.z": gems[i][2],
                                                            "scale": gems[i][3],
                                                        }))
                }

                console.log("Gems created: " + gems.length)
                painter.active = true

                scene.geometries = gemsToJSON

                if (loadScreen !== null) {
                    loadScreen.visible = false
                }
            } else if (messageObject.currentProgress) {
                loadScreen.currentProgress = messageObject.currentProgress
            }
        }

        function initGemTypes()
        {
            // Improve the solution when a configuration file is available
            var gemTypes = {}

            config.gemTypes.forEach(function(type) {
                gemTypes[type] = Qt.createComponent(type + ".qml")
            })

            return gemTypes
        }
    }

    PauseButton {
        id: pause
        onPressedChanged: {
            if (pressed) {
                painter.active = !painter.active
            }
        }
    }

    Component.onCompleted: {
        gemGenerator.sendMessage({"numGems": config.numGems,"gemRangeSize": config.gemRangeSize, "rangeStart": -Config.axisRange,
                                     "rangeEnd": Config.axisRange, "gemTypes": config.gemTypes})
    }
}

