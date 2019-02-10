# WWKeywordExtractor
This is a keyword extractor for software engineering related keywords.

# Requirements
- C++11
- Emscripten SDK (for web assembly compilation)
- Python 3

# How it is build
- There is an underlying C++11 machine learning module that is trained for keyword extraction
- The C++11 module is compiled to WebAssembly using Emscripten
- The Javascript files load the C++11 module and call it for keyword extraction