CXX = g++ -std=c++11 -DDEBUG # compiler
CXXFLAGS = -g -O2 -Wall # compiler flags

OBJECTS = maxent/maxent.o maxent/lbfgs.o maxent/owlqn.o maxent/sgd.o keyword_extr_model.o postagger_model.o util.o main.o
SOURCES = ${OBJECTS:.o=.cc}
DEPENDS = ${OBJECTS:.o=.d} # substitute ".o" with ".d"
EXEC = extractor # executable name

########## Targets ##########

.PHONY : clean # not file names

${EXEC} : ${OBJECTS} # link step
	${CXX} ${CXXFLAGS} $^ -o $@ # additional object files before $^

# make implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS} # include *.d files containing program dependences

wasm : # compile to webassembly module
	make && ./extractor train
	emcc -std=c++11 -DWASM -DDEBUG --bind -o extractor.js ${SOURCES} -O2 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s DISABLE_EXCEPTION_CATCHING=2 -s TOTAL_MEMORY=104857600 --preload-file data/bias_weights.txt --preload-file data/specified_tags.txt --preload-file data/weights.txt --preload-file data/train.txt --preload-file data/test.txt --preload-file extractor.model
	python wasmcompile.py

wasm_test : 
	emcc -std=c++11 -DDEBUG --emrun --bind -o extractor.html ${SOURCES} -O2 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s DISABLE_EXCEPTION_CATCHING=2 -s TOTAL_MEMORY=104857600 --preload-file data/bias_weights.txt --preload-file data/specified_tags.txt --preload-file data/weights.txt --preload-file data/train.txt --preload-file data/test.txt
	emrun --browser chrome extractor.htm

clean : # remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC} extractor.*