# Define required macros here
SHELL = /bin/sh

#SOURCES = hello_world.c
#SOURCE2 = hello.c
OBJDIR = src/
#OBJS=hello_world.o header/hello.o
#INCLUDE = header/hello.h
LIBS=-lm

#RELEASEDIR=/home/steam/Desktop/STEAM/STEAM_FlightSoftware/Unit_Building/testfile/hello_folder/header
INCLUDEDIR=./header
INCLUDES=-I${INCLUDEDIR}
#VPATH=-I header #-I./. -I${INCLUDEDIR}
#vpath %.c header



#OUTS = *.o

#LIBSOURCES:=*.c
#INCLUDEFILES:=*.h


SOURCES =	$(wildcard *.c)
SOURCES +=	$(wildcard $(OBJDIR)*.c)

#$(wildcard *.o): $(wildcard *.c)
#	${CC} $(CFLAGS) $(INCLUDE) -o $<


helloworld:${SOURCES}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${SOURCES} ${LIBS}


.PHONY: clean
clean:
	-rm -f *.o core *.core



