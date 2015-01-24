WorkerScript.onMessage = function(message)
{
    var gems = new Array();
    var gemTypes = message.gemTypes
    var gemRangeSize = message.gemRangeSize


    // Always generate the first available at the position (0, 0, 0)
    gems.push(new Array(0, 0, 0, getGemSize(gemRangeSize), gemTypes[0]))

    var trys = 0
    var curGemNum = 0
    var oldGemNum = 0

    while (gems.length < message.numGems) {
        if (curGemNum != oldGemNum) {
            trys = 0
            oldGemNum = curGemNum
        }

        if (trys == 50) {
            break
        }

        trys++

        var newX = Math.random() * (message.rangeEnd - message.rangeStart) + message.rangeStart
        var newY = Math.random() * (message.rangeEnd - message.rangeStart) + message.rangeStart
        var newZ = Math.random() * (message.rangeEnd - message.rangeStart) + message.rangeStart
        var isValidNewGem = true

        // Prevent gems from spawing next to each other
        var variance = 0.2
        var curSize = 0

        for (var i = 0; i < gems.length; i++) {
            curSize = getGemSize(gemRangeSize)

            if (gems[i][0] + gems[i][3] * Math.sqrt(3) > newX &&
                    gems[i][0] - gems[i][3] * Math.sqrt(3) < newX &&
                    gems[i][1] + gems[i][3] * Math.sqrt(3) > newY &&
                    gems[i][1] - gems[i][3] * Math.sqrt(3) < newY &&
                    gems[i][2] + gems[i][3] * Math.sqrt(3) > newZ &&
                    gems[i][2] - gems[i][3] * Math.sqrt(3) < newZ) {
                isValidNewGem = false
            }
        }

        if (isValidNewGem) {
            var newGem = new Array(newX, newY, newZ, curSize, getRandomGemType(gemTypes))
            gems.push(newGem)
            WorkerScript.sendMessage({"currentProgress": (gems.length / message.numGems)})

            curGemNum++
        }
    }

    WorkerScript.sendMessage({"gems": gems})
}

function getRandomGemType(gemTypes)
{
    var gemTreshold = Math.random()
    gemTreshold = Math.floor(Math.random() * gemTypes.length)

    return gemTypes[gemTreshold]
}

function getGemSize(gemRangeSize)
{
    var gemSize = Math.random()
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0]
    return gemSize
}
