WorkerScript.onMessage = function(message)
{
    var gems = new Array();
    var gemTypes = message.gemTypes
    var gemRangeSize = message.gemRangeSize
    var range = message.rangeEnd - message.rangeStart

    var numGemsPerDim = Math.ceil(Math.pow(message.numGems, 1/3));
    var x, y, z
    var gemsCompleted = false
    var gemInterval = gemRangeSize[1] * 2 * Math.sqrt(3)

    for (var i = 1; i <= numGemsPerDim && !gemsCompleted; i++) {
        for (var j = 1; j <= numGemsPerDim && !gemsCompleted; j++) {
            for (var k = 1; k <= numGemsPerDim && !gemsCompleted; k++) {
                x = i * gemInterval - (range / 2)
                y = j * gemInterval - (range / 2)
                z = k * gemInterval - (range / 2)
                gems.push([x, y, z, gemRangeSize[1], getRandomGemType(gemTypes)])

                WorkerScript.sendMessage({"currentProgress": (gems.length / message.numGems)})

                if (gems.length === message.numGems) {
                    gemsCompleted = true
                }
            }
        }
    }

    console.log(gems)

    WorkerScript.sendMessage({"gems": gems})
}

function random(seed) {
    var x = Math.sin(seed) * 10000;
    return x - Math.floor(x);
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
