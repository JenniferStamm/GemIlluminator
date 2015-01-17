WorkerScript.onMessage = function(message)
{
    var gems = new Array();

    gems.push(new Array(0, 0, 0, "gem"))

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
        var variance = 0.1

        for (var i = 0; i < gems.length; i++) {
            if (gems[i][0] + message.gemSize + variance * message.gemSize > newX &&
                    gems[i][0] - message.gemSize - variance * message.gemSize < newX &&
                    gems[i][1] + message.gemSize + variance * message.gemSize  > newY &&
                    gems[i][1] - message.gemSize - variance * message.gemSize < newY &&
                    gems[i][2] + message.gemSize + variance * message.gemSize  > newZ &&
                    gems[i][2] - message.gemSize - variance * message.gemSize < newZ) {
                isValidNewGem = false
            }
        }

        if (isValidNewGem) {
            var newGem = new Array(newX, newY, newZ, "gem")
            gems.push(newGem)
            WorkerScript.sendMessage({"currentProgress": (gems.length / message.numGems)})

            curGemNum++
        }
    }

    WorkerScript.sendMessage({"gems": gems})
}
