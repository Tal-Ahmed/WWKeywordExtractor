  #!/usr/bin/python

wasm_js_file = "extractor.js"
background_script_js_file = "keyword_extractor.js"
main_js = "compiled_background_script.js"

def main():
  # correctly read extractor.wasm and extractor.data
  with open(wasm_js_file) as f:
    wasm_js_text = f.read()\
        .replace('scriptDirectory+path', 'chrome.runtime.getURL(path)')\
        .replace('var REMOTE_PACKAGE_BASE="extractor.data";', 'var REMOTE_PACKAGE_BASE=chrome.runtime.getURL(PACKAGE_NAME);')

  with open(background_script_js_file) as f:
    background_script_js_text = f.read()

  with open(main_js, "w") as f:
    f.write(wasm_js_text + "\n" + background_script_js_text)

if __name__ == "__main__":
    main()