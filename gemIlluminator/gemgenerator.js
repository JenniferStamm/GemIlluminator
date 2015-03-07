WorkerScript.onMessage = function(message)
{
    var gems = new Array();
    var gemTypes = message.gemTypes;
    var gemRangeSize = message.gemRangeSize;
    var range = message.rangeEnd - message.rangeStart;

    var numGemsPerDim = Math.ceil(Math.pow(message.numGems, 1/3));
    var x, y, z;
    var gemsCompleted = false;
    var gemInterval = gemRangeSize[1] * 2; //* Math.sqrt(3)
    var gapAddition = (range - (numGemsPerDim * gemInterval)) / numGemsPerDim;
    gemInterval += gapAddition;

    var newGem = null;
    var randomGemTypeIndex = null;
    var gemSize = 0;
    var posVariance = 0;
    var seed = 0;

    for (var i = 1; i <= numGemsPerDim && !gemsCompleted; ++i) {
        for (var j = 1; j <= numGemsPerDim && !gemsCompleted; ++j) {
            for (var k = 1; k <= numGemsPerDim && !gemsCompleted; ++k) {
                seed = (i * k + j) * message.seed.hashCode();
                gems.push(getSeededGem(i, j, k, gemInterval, range, gemTypes, gemRangeSize, seed));

                WorkerScript.sendMessage({"currentProgress": (gems.length / message.numGems)});

                if (gems.length === message.numGems) {
                    gemsCompleted = true;
                }
            }
        }
    }

    WorkerScript.sendMessage({"gems": gems})
}

// Seeded randomize function
function random(seed)
{
    var x = Math.sin(seed) * 10000;
    return x - Math.floor(x);
}

// Get a random variance for the
function getRandomPos(posIndex, posVariance, gemInterval, range)
{
    if (Math.random() - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * Math.random());
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * Math.random());
    }
}

function getSeededPos(posIndex, posVariance, gemInterval, range, seed)
{
    if (Math.random() - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * random(seed));
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * random(seed));
    }
}

//
function getRandomGemTypeIndex(gemTypes)
{
    return Math.min(Math.floor(Math.random() * gemTypes.length), 1);
}

function getSeededGemTypeIndex(gemTypes, seed)
{
    return Math.min(Math.floor(random(seed) * gemTypes.length), 1);
}

//
function getRandomGemSize(gemRangeSize)
{
    var gemSize = Math.random();
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0];

    return gemSize;
}

function getSeededGemSize(gemRangeSize, seed)
{
    var gemSize = random(seed);
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0];

    return gemSize;
}

//
function getRandomGem(i, j, k, gemInterval, range, gemTypes, gemRangeSize)
{
    var gemSize, posVariance, x, y, z, xAngle, yAngle, zAngle, randomGemTypeIndex;

    gemSize = getRandomGemSize(gemRangeSize);
    posVariance = (gemRangeSize[1] - gemSize) / 2;

    x = getRandomPos(i, posVariance, gemInterval, range);
    y = getRandomPos(j, posVariance, gemInterval, range);
    z = getRandomPos(k, posVariance, gemInterval, range);
    xAngle = Math.rand() * 360 - 180;
    yAngle = Math.rand() * 360 - 180;
    zAngle = Math.rand() * 360 - 180;

    randomGemTypeIndex = getRandomGemTypeIndex(gemTypes);

    return [x, y, z, gemSize, xAngle, yAngle, zAngle, gemTypes[randomGemTypeIndex]]
}

function getSeededGem(i, j, k, gemInterval, range, gemTypes, gemRangeSize, seed)
{
    var gemSize, posVariance, x, y, z, xAngle, yAngle, zAngle, seededGemTypeIndex;

    gemSize = getSeededGemSize(gemRangeSize, seed);
    posVariance = (gemRangeSize[1] - gemSize) / 2;

    x = getSeededPos(i, posVariance, gemInterval, range, seed);
    y = getSeededPos(j, posVariance, gemInterval, range, seed);
    z = getSeededPos(k, posVariance, gemInterval, range, seed);
    xAngle = random(seed * i) * 360 - 180;
    yAngle = random(seed * j) * 360 - 180;
    zAngle = random(seed * k) * 360 - 180;

    seededGemTypeIndex = getSeededGemTypeIndex(gemTypes, seed);

    return [x, y, z, gemSize, xAngle, yAngle, zAngle, gemTypes[seededGemTypeIndex]];
}

// String hash function see:
String.prototype.hashCode = function()
{
    var hash = 0;
    var character = 0;
    var i = 0;

    if (this.length === 0) {
        return hash;
    }

    for (i = 0; i < this.length; i++) {
        character = this.charCodeAt(i);
        hash = ((hash << 5) - hash) + character;
        hash = hash & hash; // Convert to 32bit integer
    }

    return hash;
}
