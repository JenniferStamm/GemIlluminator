import QtQuick 2.0
/**
 * @brief Gets mouse movements while the left button is pressed and maps them to rotation.
 */
MouseArea {
    id: mouseArea
    acceptedButtons: Qt.LeftButton
    anchors.fill: parent
    enabled: false

    property var navigation
    property vector2d referencePoint: Qt.vector2d(0, 0)

    onPressed: {
        referencePoint = Qt.vector2d(mouse.x, mouse.y);
    }

    onPositionChanged: {
        if (Qt.platform.os === "windows") {
            var scale = Math.min(root.width, root.height);
            var filteredPosition = referencePoint.plus((Qt.vector2d(mouse.x, mouse.y).minus(referencePoint)).times(config.smoothnessFactor));
            var xAngle = ((filteredPosition.y - referencePoint.y) / scale) * 180; //move mouse in x direction leads to rotation to left or right
            var yAngle = ((filteredPosition.x - referencePoint.x) / scale) * 180; //move mouse in y direction leads to rotation to top or bottom
            navigation.eulerRotation = Qt.vector3d(xAngle, yAngle, 0);
            referencePoint = filteredPosition;
        }
    }
}

