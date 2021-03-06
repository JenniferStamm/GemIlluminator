import GemIlluminator 1.0

import QtQuick 2.0

/**
 * @brief QML object of the config. The file is named ConfigView.qml to prevent name collisions with the global config.
 */
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

    /**
     * @brief Takes the config file content and parses it into JSON to get config properties.
     */
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

    /**
     * @brief Writes back the config properties to the config file.
     */
    function saveConfig()
    {
        var convertedConfig = '{\n'
        convertedConfig += '\t"GemTypes": ["' + gemTypes.join('", "') + '"],\n'
        convertedConfig += '\t"NumGems": ' + numGems + ',\n'
        convertedConfig += '\t"AxisRange": ' + Config.axisRange + ',\n'
        convertedConfig += '\t"GemRangeSize": [' + gemRangeSize.join(', ') + '],\n'
        convertedConfig += '\t"AvailableEnvMaps": ["' + availableEnvMaps.join('", "') + '"],\n'
        convertedConfig += '\t"EnvMap": "' + Config.envMap + '",\n'
        convertedConfig += '\t"ViewportRatio": ' + viewportRatio + ',\n'
        convertedConfig += '\t"NavigationSmoothnessFactor": ' + smoothnessFactor + '\n'
        convertedConfig += '}\n'
        Config.write(convertedConfig)
    }
}

