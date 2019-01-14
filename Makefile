CFLAGS = -O2 -Wall
OBJS = maxent/maxent.o maxent/lbfgs.o maxent/owlqn.o maxent/sgd.o mltk/_ctagger.o mltk/_utils.o mltk/murmur3.o

all: keyword_extractor

keyword_extractor: keyword_extr_model.o postagger_model.o main.o $(OBJS)
	g++ -o extractor $(CFLAGS) $(OBJS) keyword_extr_model.o postagger_model.o main.o
clean:
	rm -f $(OBJS) *.o a.out extractor.model postagger.model extractor
*.o:
	g++ -c $(CFLAGS) $<