CFLAGS = -O2 -Wall -DNDEBUG
#CFLAGS = -g -Wall
OBJS = maxent.o lbfgs.o owlqn.o sgd.o

all: bicycle postagging

bicycle: bicycle.o $(OBJS)
	g++ -o bicycle $(CFLAGS) $(OBJS) bicycle.o
postagging: postagging.o $(OBJS)
	g++ -o postagging $(CFLAGS) $(OBJS) postagging.o
clean:
	/bin/rm -r -f $(OBJS) bicycle postagging *.o *~ model
.cpp.o:
	g++ -c $(CFLAGS) $<