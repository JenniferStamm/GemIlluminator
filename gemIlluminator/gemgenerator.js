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
    var gapAddition = (range - (numGemsPerDim * gemInterval)) / numGemsPerDim
    gemInterval += gapAddition

    var newGem = null
    var randomGemTypeIndex = null
    var gemSize = 0
    var posVariance = 0

    for (var i = 1; i <= numGemsPerDim && !gemsCompleted; ++i) {
        for (var j = 1; j <= numGemsPerDim && !gemsCompleted; ++j) {
            for (var k = 1; k <= numGemsPerDim && !gemsCompleted; ++k) {
                gemSize = getRandomGemSize(gemRangeSize)
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

// Seeded randomize function
function random(seed) {
    var x = Math.sin(seed) * 10000;
    return x - Math.floor(x);
}

// Get a random variance for the
function getRandomPos(posIndex, posVariance, gemInterval, range)
{
    if (Math.random() - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * Math.random())
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * Math.random())
    }
}

function getSeededPos(posIndex, posVariance, gemInterval, range, seed)
{
    if (Math.random() - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * random(seed))
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * random(seed))
    }
}

//
function getRandomGemTypeIndex(gemTypes)
{
    return Math.min(Math.floor(Math.random() * gemTypes.length), 1)
}

function getSeededGemTypeIndex(gemTypes, seed)
{
    return Math.min(Math.floor(random(seed) * gemTypes.length), 1)
}

//
function getRandomGemSize(gemRangeSize)
{
    var gemSize = Math.random()
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0]
    return gemSize
}

function getSeededGemSize(gemRangeSize, seed)
{
    var gemSize = random(seed)
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0]
    return gemSize
}

// String hash function see:
/*String.prototype.hashCode = function(){
    var hash = 0;

    if (this.length === 0)
    {
        return hash;
    }

    for (i = 0; i < this.length; i++) {
        char = this.charCodeAt(i);
        hash = ((hash << 5) - hash) + char;
        hash = hash & hash; // Convert to 32bit integer
    }

    return hash;
}*/
