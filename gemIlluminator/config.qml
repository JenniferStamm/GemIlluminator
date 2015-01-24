import QtQuick 2.0
import QtQuick.LocalStorage 2.0

Item {
    id: config
    visible: false
    property var gemTypes: null
    property int numGems: 0
    property var gemRangeSize: null

    Component.onCompleted: {
        loadConfig()
    }

    function loadConfig()
    {
        var request = new XMLHttpRequest()
        request.open('GET', 'config.json')
        request.onreadystatechange = function(event) {
            if (request.readyState == XMLHttpRequest.DONE) {
                _parseConfig(JSON.parse(request.responseText))
            }
        }
        request.send()
    }

    function _parseConfig(config)
    {
        gemTypes = config["GemTypes"]
        numGems = config["NumGems"]
        gemRangeSize = config["GemRangeSize"]
    }
}

