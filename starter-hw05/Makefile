
SRCS := $(wildcard *.c)
OBJS :=$(SRCS:.c=.o)
HDRS := $(wildcard *.h)

tokens: $(OBJS)
	gcc -g -o tokens $(OBJS)

%.o: %.c $(HDRS)
	gcc -c  -g -o $@ $<

clean: 
	rm -f tokens *.o

.PHONY: clean
	
