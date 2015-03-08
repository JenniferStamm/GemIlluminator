import GemIlluminator 1.0

import QtQuick 2.0

Item {
    id: config
    property var gemTypes: null
    property int numGems: 0
    property var gemRangeSize: null
    property var availableEnvMaps: null
    property int viewportRatio: 0
    property double smoothnessFactor: 0

    Component.onCompleted: {
        Config.source = "config.json"
        loadConfig()
    }

    function loadConfig()
    {
        var configJSON = JSON.parse(Config.read())
        gemTypes = configJSON["GemTypes"]
        numGems = configJSON["NumGems"]
        gemRangeSize = configJSON["GemRangeSize"]
        availableEnvMaps = configJSON["AvailableEnvMaps"]
        viewportRatio = configJSON["ViewportRatio"]
        smoothnessFactor = configJSON["NavigationSmoothnessFactor"]

        Config.axisRange = configJSON["AxisRange"]
        Config.envMap = configJSON["EnvMap"]
        Config.maxGemSize = gemRangeSize[1]
        Config.minGemSize = gemRangeSize[0]
    }

    function saveConfig()
    {
        var convertedConfig = '{\n'
        convertedConfig += '\t"GemTypes": ["' + gemTypes.join('", "') + '"],\n'
        convertedConfig += '\t"NumGems": ' + numGems + ',\n'
        convertedConfig += '\t"AxisRange": ' + Config.axisRange + ',\n'
        convertedConfig += '\t"GemRangeSize": [' + gemRangeSize.join(', ') + '],\n'
        convertedConfig += '\t"AvailableEnvMaps": ["' + availableEnvMaps.join('", "') + '"],\n'
        convertedConfig += '\t"EnvMap": "' + Config.envMap + ',"\n'
        convertedConfig += '\t"ViewportRatio": ' + viewportRatio + '\n'
        convertedConfig += '}\n'
        Config.write(convertedConfig)
    }
}

