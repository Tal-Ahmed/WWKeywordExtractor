var last;

/**
 * Simple Javascript DOM Selector
 * Get full CSS path of any element
 * 
 * Returns a jQuery-style CSS path, with IDs, classes and ':nth-child' pseudo-selectors.
 * 
 * Can either build a full CSS path, from 'html' all the way to ':nth-child()', or a
 * more optimised short path, stopping at the first parent with a specific ID,
 * eg. "#content .top p" instead of "html body #main #content .top p:nth-child(3)"
 */
function cssPath(el) {
    var fullPath    = 0,  // Set to 1 to build ultra-specific full CSS-path, or 0 for optimised selector
    useNthChild = 0,  // Set to 1 to use ":nth-child()" pseudo-selectors to match the given element
    cssPathStr = '',
    testPath = '',
    parents = [],
    parentSelectors = [],
    tagName,
    cssId,
    cssClass,
    tagSelector,
    vagueMatch,
    nth,
    i,
    c;

    // Go up the list of parent nodes and build unique identifier for each:
    while ( el ) {
        vagueMatch = 0;

        // Get the node's HTML tag name in lowercase:
        tagName = el.nodeName.toLowerCase();

        // Get node's ID attribute, adding a '#':
        cssId = ( el.id ) ? ( '#' + el.id ) : false;

        // Get node's CSS classes, replacing spaces with '.':
        cssClass = ( el.className ) ? ( '.' + el.className.replace(/\s+/g,".") ) : '';

        // Build a unique identifier for this parent node:
        if ( cssId ) {
            // Matched by ID:
            tagSelector = tagName + cssId + cssClass;
        } else if ( cssClass ) {
            // Matched by class (will be checked for multiples afterwards):
            tagSelector = tagName + cssClass;
        } else {
            // Couldn't match by ID or class, so use ":nth-child()" instead:
            vagueMatch = 1;
            tagSelector = tagName;
        }

        // Add this full tag selector to the parentSelectors array:
        parentSelectors.unshift( tagSelector )

        // If doing short/optimised CSS paths and this element has an ID, stop here:
        if ( cssId && !fullPath )
            break;

        // Go up to the next parent node:
        el = el.parentNode !== document ? el.parentNode : false;

    } // endwhile


    // Build the CSS path string from the parent tag selectors:
    for ( i = 0; i < parentSelectors.length; i++ ) {
        cssPathStr += ' ' + parentSelectors[i];// + ' ' + cssPathStr;

        // If using ":nth-child()" selectors and this selector has no ID / isn't the html or body tag:
        if ( useNthChild && !parentSelectors[i].match(/#/) && !parentSelectors[i].match(/^(html|body)$/) ) {

            // If there's no CSS class, or if the semi-complete CSS selector path matches multiple elements:
            if ( !parentSelectors[i].match(/\./) || $( cssPathStr ).length > 1 ) {
                
                // Count element's previous siblings for ":nth-child" pseudo-selector:
                for ( nth = 1, c = el; c.previousElementSibling; c = c.previousElementSibling, nth++ );
                
                // Append ":nth-child()" to CSS path:
                cssPathStr += ":nth-child(" + nth + ")";
            }
        }

    }

    // Return trimmed full CSS path:
    return cssPathStr.replace(/^[ \t]+|[ \t]+$/, '');
}

/**
 * MouseOver action for all elements on the page:
 */
function inspectorMouseOver(e) {
    // NB: this doesn't work in IE (needs fix):
    var element = e.target;

    // Set outline:
    element.style.outline = '2px solid #f00';

    // Set last selected element so it can be 'deselected' on cancel.
    last = element;
}

/**
 * MouseOut event action for all elements
 */
function inspectorMouseOut(e) {
    // Remove outline from element:
    e.target.style.outline = '';
}

function getParsedURL(){
    var url = window.location.href;
    if (url.charAt(url.length - 1) == "/"){
        url = url.substring(0, url.length - 1);
    }

    url = url.substring(0, url.lastIndexOf("/") + 1);
    return url;
}

/**
 * Click action for hovered element
 */
function inspectorOnClick(e) {
    e.preventDefault();
    inspectorCancel();

    fetch(chrome.extension.getURL("popup.html"))
    .then(response => response.text())
    .then(data => {
        document.body.innerHTML += data;
        fetch(chrome.extension.getURL("popup.css"))
        .then(response => response.text())
        .then(data => {
            var styleElement = document.createElement("style");
            styleElement.setAttribute("type", "text/css");
            styleElement.setAttribute("id", "ww-extractor-style");
            styleElement.innerHTML = data;
            document.head.appendChild(styleElement);

            // set url
            var urlInputElement = document.getElementsByClassName("ww-extractor-site-selector-input")[0];
            var url = getParsedURL();
            urlInputElement.value = url + ".*";

            // set css path of selected element
            var cssSelectorInputElement = document.getElementsByClassName("ww-extractor-css-selector-input")[0];
            cssSelectorInputElement.value = cssPath(e.target);

            // set on cancel button click
            var cancelButtonElement = document.getElementsByClassName("ww-extractor-cancel-button")[0];
            cancelButtonElement.onclick = function(){
                var aside = document.getElementById("ww-extractor-aside");
                aside.parentNode.removeChild(aside);

                var style = document.getElementById("ww-extractor-style");
                style.parentNode.removeChild(style);
            };

            // set on extractor button click
            var extractorButtonElement = document.getElementsByClassName("ww-extractor-extract-button")[0];
            extractorButtonElement.onclick = function(){
                var urlToStore = url;
                var cssSelectorToStore = cssSelectorInputElement.value;

                chrome.storage.sync.get([urlToStore], function(data){
                    if (!(urlToStore in data)){
                        chrome.storage.sync.set({[urlToStore]: [cssSelectorToStore]}, function() {});
                    } else {
                        if (!data[[urlToStore]].includes(cssSelectorToStore)){
                            data[[urlToStore]].push(cssSelectorToStore);
                            chrome.storage.sync.set({[urlToStore]: data.urlToStore});
                        }
                    }

                    generate_keywords();
                });

                var aside = document.getElementById("ww-extractor-aside");
                aside.parentNode.removeChild(aside);

                var style = document.getElementById("ww-extractor-style");
                style.parentNode.removeChild(style);
            };
        });
    });

    return false;
}

/**
 * Function to cancel inspector:
 */
function inspectorCancel(e) {
    document.body.style.cursor = "default";
    // Unbind inspector mouse and click events:
    if (e === null && event.keyCode === 27) { // IE (won't work yet):
        document.detachEvent("mouseover", inspectorMouseOver);
        document.detachEvent("mouseout", inspectorMouseOut);
        document.detachEvent("click", inspectorOnClick);
        document.detachEvent("keydown", inspectorCancel);
        last.style.outlineStyle = 'none';
    } else if(e == null || e.which === 27) { // Better browsers:
        document.removeEventListener("mouseover", inspectorMouseOver, true);
        document.removeEventListener("mouseout", inspectorMouseOut, true);
        document.removeEventListener("click", inspectorOnClick, true);
        document.removeEventListener("keydown", inspectorCancel, true);

        // Remove outline on last-selected element:
        last.style.outline = 'none';
    }
}

/**
 * Add event listeners for DOM-inspectorey actions
 */
function runInspector(){
    document.body.style.cursor = "crosshair";
    if ( document.addEventListener ) {
        document.addEventListener("mouseover", inspectorMouseOver, true);
        document.addEventListener("mouseout", inspectorMouseOut, true);
        document.addEventListener("click", inspectorOnClick, true);
        document.addEventListener("keydown", inspectorCancel, true);
    } else if ( document.attachEvent ) {
        document.attachEvent("mouseover", inspectorMouseOver);
        document.attachEvent("mouseout", inspectorMouseOut);
        document.attachEvent("click", inspectorOnClick);
        document.attachEvent("keydown", inspectorCancel);
    }
}

chrome.runtime.onMessage.addListener(function(request, sender, sendResponse) {
    if (request.runInspector){
        runInspector();
    }
});

function extractKeywords(cssSelectors){
    Extractor().then(function(Module) {
        for (var i = 0; i < cssSelectors.length; i += 1){
            var cssSelector = cssSelectors[i];
            var element = document.querySelector(cssSelector);
            if (element != null){
                var lines = element.innerHTML.split("<br>");
                var replaceWithInnerHTML = [];
                for (var j = 0; j < lines.length; j += 1){
                    var line = lines[j];
                    var keywordTokens = Module.generate_keywords(line);

                    for (var k = 0; k < keywordTokens.size(); k += 1){
                        var keywordToken = keywordTokens.get(k);
                        if (keywordToken.is_keyword){
                            replaceWithInnerHTML.push("<mark>" + keywordToken.word + "</mark>");
                        } else {
                            replaceWithInnerHTML.push(keywordToken.word);
                        }
                        
                        if (k + 1 < keywordTokens.size()) replaceWithInnerHTML.push(" ");
                    }

                    if (j + 1 < lines.length){
                        replaceWithInnerHTML.push("<br>");
                    }
                }

                element.innerHTML = replaceWithInnerHTML.join("");
            }
        }
    });
}

// check to see if we need to extract keywords from this site
function generate_keywords(){
    chrome.storage.sync.get(null, function(data){
        for (var url in data){        
            if (data.hasOwnProperty(url)){
                try {
                    var reg = new RegExp(url.replace(/[.*+?^${}()|[\]\\]/g, '\\$&') + ".*");
                    if (reg.test(window.location.href)){
                        extractKeywords(data[url]);
                    }    
                } catch (e){}
            }
        }
    });
}

generate_keywords();