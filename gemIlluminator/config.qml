import QtQuick 2.0
import GemIlluminator 1.0

Config {
    id: config
    source: "config.json"
    property var gemTypes: null
    property int numGems: 0
    property var gemRangeSize: null

    Component.onCompleted: {
        console.log(read())
    }

    function _parseConfig(config)
    {
        gemTypes = config["GemTypes"]
        numGems = config["NumGems"]
        gemRangeSize = config["GemRangeSize"]
    }
}

