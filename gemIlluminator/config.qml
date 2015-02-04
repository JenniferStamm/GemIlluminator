import QtQuick 2.0
import GemIlluminator 1.0

Config {
    id: config
    source: "config.json"
    property var gemTypes: null
    property int numGems: 0
    property var gemRangeSize: null

    Component.onCompleted: {
        loadConfig()
    }

    function loadConfig()
    {
        var configJSON = JSON.parse(read())
        gemTypes = configJSON["GemTypes"]
        numGems = configJSON["NumGems"]
        gemRangeSize = configJSON["GemRangeSize"]
    }
}

