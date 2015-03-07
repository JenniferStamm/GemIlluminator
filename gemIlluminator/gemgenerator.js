WorkerScript.onMessage = function(message)
{
    var gems = new Array();
    var gemTypes = message.gemTypes
    var gemRangeSize = message.gemRangeSize
    var range = message.rangeEnd - message.rangeStart

    var numGemsPerDim = Math.ceil(Math.pow(message.numGems, 1/3));
    var x, y, z
    var gemsCompleted = false
    var gemInterval = gemRangeSize[1] * 2 //* Math.sqrt(3)
    var gapFactor = (range - (numGemsPerDim * gemInterval)) / numGemsPerDim
    gemInterval += gapFactor

    var newGem = null
    var randomGemTypeIndex = null
    var gemSize = 0
    var posVariance = 0

    gemSize = getGemSize(gemRangeSize)
    posVariance = (gemRangeSize[1] - gemSize) / 2

    x = 0
    y = 0
    z = 0

    randomGemTypeIndex = getRandomGemTypeIndex(gemTypes)
    newGem = [x, y, z, gemSize, gemTypes[randomGemTypeIndex]]
    gems.push(newGem)

    for (var i = 1; i <= numGemsPerDim && !gemsCompleted; i++) {
        for (var j = 1; j <= numGemsPerDim && !gemsCompleted; j++) {
            for (var k = 1; k <= numGemsPerDim && !gemsCompleted; k++) {
                gemSize = getGemSize(gemRangeSize)
                posVariance = (gemRangeSize[1] - gemSize) / 2

                x = getRandomPos(i, posVariance, gemInterval, range)
                y = getRandomPos(j, posVariance, gemInterval, range)
                z = getRandomPos(k, posVariance, gemInterval, range)

                randomGemTypeIndex = getRandomGemTypeIndex(gemTypes)
                newGem = [x, y, z, gemSize, gemTypes[randomGemTypeIndex]]
                gems.push(newGem)

                WorkerScript.sendMessage({"currentProgress": (gems.length / message.numGems)})

                if (gems.length === message.numGems) {
                    gemsCompleted = true
                }
            }
        }
    }

    WorkerScript.sendMessage({"gems": gems})
}

function random(seed) {
    var x = Math.sin(seed) * 10000;
    return x - Math.floor(x);
}

function getRandomPos(posIndex, posVariance, gemInterval, range)
{
    if (Math.random() - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * Math.random())
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * Math.random())
    }
}

function getRandomGemTypeIndex(gemTypes)
{
    return Math.min(Math.floor(Math.random() * gemTypes.length), 1)
}

function getGemSize(gemRangeSize)
{
    var gemSize = Math.random()
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0]
    return gemSize
}
