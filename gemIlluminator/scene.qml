import QtGraphicalEffects 1.0
import QtQuick 2.0
import QtQuick.Window 2.2
import GemIlluminator 1.0
import "gemgenerator.js" as GemGenerator

Scene {
    id: scene
    property alias cameraId: camera
    property var loadScreen: null
    property int score: 0
    anchors.fill: parent

    onGameLost: {
        timer.stop()
        gameOver.visible = true
    }

    onGameStarted: {
        pause.visible = true
        highscore.visible = true
        painter.resetTimer()
        score = 0
        timer.start()
    }

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

    Timer {
        id: timer
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            score = score + 1
            highscore.update()
        }
    }

    Text {
        id: highscore
        visible: false
        color: "white"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 16
        text: "Score: " + score
    }

    PauseButton {
        id: pause
        visible: false
        onPressedChanged: {
            if (pressed) {
                painter.active = !painter.active
                if (painter.active) {
                    timer.start()
                } else {
                    timer.stop()
                }
            }
        }
    }

    Component.onCompleted: {
        gemGenerator.sendMessage({"numGems": config.numGems,"gemRangeSize": config.gemRangeSize, "rangeStart": -Config.axisRange,
                                     "rangeEnd": Config.axisRange, "gemTypes": config.gemTypes})
    }

    Rectangle {
        id: gameOver
        visible: false
        anchors.fill: parent
        color: "transparent"

        RadialGradient {
            anchors.fill: parent
            cached: true
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#70000000" }
                GradientStop { position: 0.325; color: "#D0000000" }
                GradientStop { position: 0.45; color: "#FF000000" }
            }
        }

        Text {
            id: gameOverText
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: Screen.pixelDensity * 20
            text: "Game Over"
            font.pointSize: 24
            color: "white"
        }

        Text {
            anchors.top: gameOverText.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: Screen.pixelDensity * 2
            text: "Score: " + score
            font.pointSize: 18
            color: "white"
        }
    }
}

