  #!/usr/bin/python

wasm_js_file = "extractor.js"

def main():
  with open(wasm_js_file) as f:
    newText = f.read().replace('scriptDirectory+path', 'chrome.runtime.getURL(path)')

  with open(wasm_js_file, "w") as f:
    f.write(newText)

if __name__ == "__main__":
    main()