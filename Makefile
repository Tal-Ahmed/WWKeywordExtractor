CFLAGS = -O2 -Wall
OBJS = maxent/maxent.o maxent/lbfgs.o maxent/owlqn.o maxent/sgd.o

all: keyword_extractor postagger

keyword_extractor: keyword_extr_model.o $(OBJS)
	g++ -o bicycle $(CFLAGS) $(OBJS) keyword_ext_model.o
postagging: postagger_model.o $(OBJS)
	g++ -o postagging $(CFLAGS) $(OBJS) postagger_model.o
clean:
	rm -f $(OBJS) a.out extractor.model postagger.model
.cpp.o:
	g++ -c $(CFLAGS) $<