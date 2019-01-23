CXX = g++ -std=c++11 # compiler
CXXFLAGS = -g -O2 -Wall # compiler flags

OBJECTS = maxent/maxent.o maxent/lbfgs.o maxent/owlqn.o maxent/sgd.o keyword_extr_model.o postagger_model.o main.o
DEPENDS = ${OBJECTS:.o=.d} # substitute ".o" with ".d"
EXEC = extractor # executable name

########## Targets ##########

.PHONY : clean # not file names

${EXEC} : ${OBJECTS} # link step
	${CXX} ${CXXFLAGS} $^ -o $@ # additional object files before $^

# make implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS} # include *.d files containing program dependences

clean : # remove files that can be regenerated
	rm -f ${DEPENDS} ${OBJECTS} ${EXEC} extractor.model