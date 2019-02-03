chrome.tabs.query({ currentWindow: true, active: true }, function (tabs) {
    var tabId = tabs[0].id;
    var port = chrome.tabs.connect(tabId);
    port.postMessage({runInspector: true});
});