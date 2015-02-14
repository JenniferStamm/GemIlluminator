import QtQuick 2.0
import GemIlluminator 1.0

Config {
    id: config
    source: "config.json"
    property var gemTypes: null
    property int numGems: 0
    property var gemRangeSize: null
    property string envMap: ""
    property int viewportRatio: 0

    Component.onCompleted: {
        loadConfig()
    }

    function loadConfig()
    {
        var configJSON = JSON.parse(read())
        gemTypes = configJSON["GemTypes"]
        numGems = configJSON["NumGems"]
        gemRangeSize = configJSON["GemRangeSize"]
        envMap = configJSON["EnvMap"]
        viewportRatio = configJSON["ViewportRatio"]
    }

    function saveConfig()
    {
        var convertedConfig = '{\n'
        convertedConfig += '\t"GemTypes": ["' + gemTypes.join('", "') + '"],\n'
        convertedConfig += '\t"NumGems": ' + numGems + ',\n'
        convertedConfig += '\t"GemRangeSize": [' + gemRangeSize.join(', ') + '],\n'
        convertedConfig += '\t"EnvMap": "' + envMap + '",\n'
        convertedConfig += '\t"ViewportRatio": ' + viewportRatio + '\n'
        convertedConfig += '}\n'
        write(convertedConfig)
    }
}

