function generateGems(numGems, gemSize, rangeStart, rangeEnd)
{
    var gems = new Array();

    gems.push(new Array(Math.random() * (rangeEnd - rangeStart) + rangeStart,
                        Math.random() * (rangeEnd - rangeStart) + rangeStart,
                        Math.random() * (rangeEnd - rangeStart) + rangeStart))

    var trys = 0
    var curGemNum = 0
    var oldGemNum = 0

    while (gems.length < numGems) {
        if (curGemNum != oldGemNum) {
            trys = 0
            oldGemNum = curGemNum
        }

        if (trys == 50) {
            break
        }

        trys++

        var newX = Math.random() * (rangeEnd - rangeStart) + rangeStart
        var newY = Math.random() * (rangeEnd - rangeStart) + rangeStart
        var newZ = Math.random() * (rangeEnd - rangeStart) + rangeStart
        var isValidNewGem = true

        // Prevent gems from spawing next to each other
        var variance = 0.1

        for (var i = 0; i < gems.length; i++) {
            if (gems[i][0] + gemSize + variance * gemSize > newX && gems[i][0] - gemSize - variance * gemSize < newX &&
                    gems[i][1] + gemSize + variance * gemSize  > newY && gems[i][1] - gemSize - variance * gemSize < newY &&
                    gems[i][2] + gemSize + variance * gemSize  > newZ && gems[i][2] - gemSize - variance * gemSize < newZ) {
                isValidNewGem = false
            }
        }

        if (isValidNewGem) {
            gems.push(new Array(newX, newY, newZ))

            curGemNum++
        }
    }

    return gems
}
