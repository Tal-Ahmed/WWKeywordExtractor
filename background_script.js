var module = null;
Extractor().then(function(Module){
    Module.generate_keywords("");
    module = Module;
    console.log("loaded extractor module into cache");
});

chrome.browserAction.onClicked.addListener(function(tab) {
    chrome.tabs.sendMessage(tab.id, {runInspector: true}, function(response) {});
});

chrome.runtime.onMessage.addListener(function(request, sender, sendResponse) {
    if (request.extractKeywords){
        var lines = request.parseLines;
        var keywordLines = [];

        for (var i = 0; i < lines.length; i += 1){
            var line = lines[i];
            var tokens = module.generate_keywords(line);
            var keywordLine = [];
    
            for (var k = 0; k < tokens.size(); k += 1){
                var keywordToken = tokens.get(k);
                keywordLine.push({
                    word: keywordToken.word,
                    is_keyword: keywordToken.is_keyword
                });
            }

            keywordLines.push(keywordLine);
        }

        sendResponse({keywordLines: keywordLines});
    }
    return true;
});