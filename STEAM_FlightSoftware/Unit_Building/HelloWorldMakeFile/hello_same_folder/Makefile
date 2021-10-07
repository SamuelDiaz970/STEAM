# Define required macros here
#SHELL = /bin/sh

#hello_SOURCES = hello.c hello_world.c
#SRCS=hello.c hello_world.c
#OBJS=hello_world.c hello.c
#CFLAG = -Wall -g
#CC = gcc# hello_world.c hello.c -o helloworld
INCLUDE = #hello.h
LIBS=-lm

RELEASEDIR=../src
INCLUDEDIR=src
INCLUDES=-I./. -I${INCLUDEDIR}

OUTS = *.o

LIBSOURCES:=*.c
INCLUDEFILES:=*.h

OBJS =	$(wildcard *.c)#$(addsuffix .o, $(basename $(wildcard *.c)))

helloworld:${OBJ}
	${CC} ${CFLAGS} ${INCLUDE} -o $@ ${OBJS} ${LIBS}

.c:
	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	-rm -f *.o core *.core




#.c:
#	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

#//.c.o:
#//   $(CC) $(CFLAGS) -c $<

#//alternatively

#//.cpp.o:
#//   $(CC) $(CFLAGS) -c $*.cpp
