  #!/usr/bin/python

wasm_js_file = "extractor.js"

def main():
  with open(wasm_js_file) as f:
    newText = f.read().replace('scriptDirectory+path', 'chrome.runtime.getURL(path)')

  with open(wasm_js_file, "w") as f:
    f.write(newText)

  with open(wasm_js_file) as f:
    newText = f.read().replace('var REMOTE_PACKAGE_BASE="extractor.data";', \
      'var REMOTE_PACKAGE_BASE=chrome.runtime.getURL(PACKAGE_NAME);')

  with open(wasm_js_file, "w") as f:
    f.write(newText)

if __name__ == "__main__":
    main()