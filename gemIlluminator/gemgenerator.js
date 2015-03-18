/**
 * @brief Asynchronous script to generate gems.
 * @detail The script runs in a seperate thread. This way the UI can be update over time
 * to show the progress of the generation.
 * @note The function has no return value, it sends a message from time to time to communicate with
 * the caller.
 * @note The function can switch between two modes.
 * - mode 1: All values are calculated randomly. (getRandomGem(...))
 * - mode 2: All values are calculated based on a given seed. (getSeededGem(...))
 * @param message A JSON object containing all parameters of the called function.
 */
WorkerScript.onMessage = function(message)
{
    var gems = new Array();
    var gemTypes = message.gemTypes;
    var gemRangeSize = message.gemRangeSize;
    var range = message.rangeEnd - message.rangeStart;

    var numGemsPerDim = Math.ceil(Math.pow(message.numGems, 1/3));
    var x, y, z;
    var gemsCompleted = false;
    var gemInterval = gemRangeSize[1] * 2;
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
                // gems.push(getRandomGem(i, j, k, gemInterval, range, gemTypes, gemRangeSize, seed));
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

/**
 * @brief Simulates randomness through a function with a seed.
 * @param seed A string that is used generate a random number.
 * @return Returns a float between 0 and 1.
 */
function random(seed)
{
    var x = Math.sin(seed) * 10000;
    return x - Math.floor(x);
}

/**
 * @brief Calculates a position for given gem parameters.
 * @param posIndex Position along an axis in the grid.
 * @param posVariance Describes how much the position along the axis varies.
 * @param gemInterval Average distance between two gems in the scene.
 * @param range The size of the area a gem can be created.
 * @return Returns a position in a range with a given position.
 */
function getRandomPos(posIndex, posVariance, gemInterval, range)
{
    if (Math.random() - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * Math.random());
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * Math.random());
    }
}

/**
 * @brief Calculates a position for given gem parameters based on a seed.
 * @param posIndex Position along an axis in the grid.
 * @param posVariance Describes how much the position along the axis varies.
 * @param gemInterval Average distance between two gems in the scene.
 * @param range The size of the area a gem can be created.
 * @param seed A string that is used generate random numbers.
 * @return Returns a position in a range with a given position based on a seed.
 */
function getSeededPos(posIndex, posVariance, gemInterval, range, seed)
{
    var rand = random(seed);

    if (rand - 0.5 > 0) {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) + posVariance * rand);
    } else {
        return (((posIndex - 0.5) * gemInterval - (range / 2)) - posVariance * rand);
    }
}

/**
 * @brief Selects a random gem from a given array of gems.
 * @param gemTypes An array of all available gem types.
 * @return Returns the index of the chosen gem.
 */
function getRandomGemTypeIndex(gemTypes)
{
    return Math.min(Math.floor(Math.random() * gemTypes.length), 1);
}

/**
 * @brief Selects a random gem from a given array of gems based on a seed.
 * @param gemTypes An array of all available gem types.
 * @param seed A string that is used generate random numbers.
 * @return Returns the index of the chosen gem.
 */
function getSeededGemTypeIndex(gemTypes, seed)
{
    return Math.min(Math.floor(random(seed) * gemTypes.length), gemTypes.length - 1);
}

/**
 * @brief Calculates a gem size based on a range.
 * @param gemRangeSize The range in which the size of a gem can alternate.
 * @return Returns a float between gemRangeSize[0] and gemRangeSize[1].
 */
function getRandomGemSize(gemRangeSize)
{
    var gemSize = Math.random();
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0];

    return gemSize;
}

/**
 * @brief Calculates a gem size based on a range and a seed.
 * @param gemRangeSize The range in which the size of a gem can alternate.
 * @param seed A string that is used generate random numbers.
 * @return Returns a float between gemRangeSize[0] and gemRangeSize[1].
 */
function getSeededGemSize(gemRangeSize, seed)
{
    var gemSize = random(seed);
    gemSize = (gemRangeSize[1] - gemRangeSize[0]) * gemSize + gemRangeSize[0];

    return gemSize;
}

/**
 * @brief Generates a gem with the given parameters.
 * @param i The x coordinate in the gem grid.
 * @param j The y coordinate in the gem grid.
 * @param k The z coordinate in the gem grid.
 * @param gemInterval Average distance between two gems in the scene.
 * @param range The size of the area a gem can be created.
 * @param gemTypes An array of all available gem types.
 * @param gemRangeSize The range in which the size of a gem can alternate.
 * @return Returns an array with all parameters for a new gem.
 */
function getRandomGem(i, j, k, gemInterval, range, gemTypes, gemRangeSize)
{
    var gemSize, posVariance, x, y, z, xAngle, yAngle, zAngle, randomGemTypeIndex, newGem;

    gemSize = getRandomGemSize(gemRangeSize);
    posVariance = (gemRangeSize[1] - gemSize) / 2;

    x = getRandomPos(i, posVariance, gemInterval, range);
    y = getRandomPos(j, posVariance, gemInterval, range);
    z = getRandomPos(k, posVariance, gemInterval, range);
    xAngle = Math.rand() * 360 - 180;
    yAngle = Math.rand() * 360 - 180;
    zAngle = Math.rand() * 360 - 180;

    randomGemTypeIndex = getRandomGemTypeIndex(gemTypes);

    newGem = [x, y, z, gemSize, xAngle, yAngle, zAngle, gemTypes[randomGemTypeIndex]];

    return newGem;
}

/**
 * @brief Generates a gem with the given parameters and a seed.
 * @param i The x coordinate in the gem grid.
 * @param j The y coordinate in the gem grid.
 * @param k The z coordinate in the gem grid.
 * @param gemInterval Average distance between two gems in the scene.
 * @param range The size of the area a gem can be created.
 * @param gemTypes An array of all available gem types.
 * @param gemRangeSize The range in which the size of a gem can alternate.
 * @param seed A string that is used generate random numbers.
 * @return Returns an array with all parameters for a new gem.
 */
function getSeededGem(i, j, k, gemInterval, range, gemTypes, gemRangeSize, seed)
{
    var gemSize, posVariance, x, y, z, xAngle, yAngle, zAngle, seededGemTypeIndex, newGem;

    gemSize = getSeededGemSize(gemRangeSize, seed);
    posVariance = (gemRangeSize[1] - gemSize) / 2;

    x = getSeededPos(i, posVariance, gemInterval, range, seed);
    y = getSeededPos(j, posVariance, gemInterval, range, seed);
    z = getSeededPos(k, posVariance, gemInterval, range, seed);
    xAngle = random(seed * i) * 360 - 180;
    yAngle = random(seed * j) * 360 - 180;
    zAngle = random(seed * k) * 360 - 180;

    seededGemTypeIndex = getSeededGemTypeIndex(gemTypes, seed);

    newGem = [x, y, z, gemSize, xAngle, yAngle, zAngle, gemTypes[seededGemTypeIndex]];

    return newGem;
}

/**
 * @brief Converts a given string into a 32 bit integer.
 * @return Returns the integer value of the string.
 */
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
