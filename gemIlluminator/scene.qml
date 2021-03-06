import GemIlluminator 1.0
import QtQuick 2.0
import QtQuick.Window 2.2
import "gemgenerator.js" as GemGenerator

/**
 * @brief QML object of the scene. This object is created for every game.
 */
Scene {
    id: scene
    anchors.fill: parent

    property alias cameraId: camera
    property var loadScreen: null
    property int score: 0

    signal painterActiveChanged()

    onGameLost: {
        if (timer.running) {
            timer.stop();
            gameOver.finalScore = score;
            gameOver.visible = true;
            highscore.visible = false;
            pause.visible = false;
        }
    }

    onGameStarted: {
        pause.visible = true;
        highscore.visible = true;
        gameOver.visible = false;
        painter.resetTimer();
        score = 0;
        timer.start();
        playerVelocityTimer.start();
    }

    onPainterActiveChanged: {
        if (painter.isActive) {
            timer.start();
            playerVelocityTimer.start();
        } else {
            timer.stop();
            playerVelocityTimer.stop();
        }
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
        startPosition: "-" + Config.axisRange + ", -" + Config.axisRange + ", -" + (Config.axisRange / 2)
        endPosition: "0, 0, 0"
        player: player
        scene: scene
    }

    WorkerScript {
        id: gemGenerator
        source: "gemgenerator.js"

        onMessage: {
            if(messageObject.gems) {
                var gems = messageObject.gems;
                var gemTypes = initGemTypes();

                var gemsToJSON = [];
                var curGemType = null;

                for (var i = 0; i < gems.length; i++) {
                    curGemType = gems[i][7].toString();
                    gemsToJSON.push(gemTypes[curGemType].createObject(scene,
                                                        {"id": "gem" + i.toString(),
                                                            "position.x": gems[i][0],
                                                            "position.y": gems[i][1],
                                                            "position.z": gems[i][2],
                                                            "scale": gems[i][3],
                                                            "xAngle": gems[i][4],
                                                            "yAngle": gems[i][5],
                                                            "zAngle": gems[i][6],
                                                        }));
                }

                painter.isGameActive = true;
                scene.geometries = gemsToJSON;

                if (loadScreen !== null) {
                    loadScreen.visible = false;
                }

                console.log("Gems created: " + gems.length)
            } else if (messageObject.currentProgress) {
                // The gem generation is only the first part of the game preparations
                loadScreen.currentProgress = messageObject.currentProgress;
            }
        }

        /**
         * @brief Creates a component for every given gem type to speed up the object creation.
         */
        function initGemTypes()
        {
            var gemTypes = {};

            config.gemTypes.forEach(function(type) {
                gemTypes[type] = Qt.createComponent(type + ".qml");
            });

            return gemTypes;
        }
    }

    Timer {
        id: timer
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            score = score + 1
            highscore.update()
        }
    }

    Timer {
        // Increase player velocity over time
        // Increase by 0.1 every three seconds
        // Final velocity is set to 10.0
        id: playerVelocityTimer
        interval: 3000
        running: false
        repeat: true
        onTriggered: {
            if (player.velocity <= 10.0) {
                player.velocity += 0.1
                if (player.velocity > 10.0) {
                    console.log("Final velocity reached!")
                }
            }
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
                painter.isGameActive = !painter.isGameActive

                if (painter.isGameActive) {
                    timer.start();
                    pause.image.source = "qrc:/data/pauseButton.png";
                } else {
                    timer.stop();
                    pause.image.source = "qrc:/data/playButton.png";
                }
            }
        }
    }

    Component.onCompleted: {
        // Take a given seed or generate a random one.
        var seed = (painter.seed.length > 0) ? painter.seed : Math.random().toString();
        // Start the gem generation with the loaded parameters.
        gemGenerator.sendMessage({"numGems": config.numGems,
                                     "gemRangeSize": config.gemRangeSize,
                                     "rangeStart": -Config.axisRange,
                                     "rangeEnd": Config.axisRange,
                                     "gemTypes": config.gemTypes,
                                     "seed": seed
                                 });
        painter.isActiveChanged.connect(painterActiveChanged);
    }
}

