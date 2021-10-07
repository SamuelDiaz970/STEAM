# Define required macros here
SHELL = /bin/sh

#SOURCES = hello_world.c
#OBJS=hello.c
INCLUDE = #hello.h
LIBS=-lm

#RELEASEDIR=/home/steam/Desktop/STEAM/STEAM_FlightSoftware/Unit_Building/testfile/hello_folder/header
#INCLUDEDIR=
#INCLUDES=-I./. -I${INCLUDEDIR}

#sources: 
#	$(MAKE) -C header

#OUTS = *.o

#LIBSOURCES:=*.c
#INCLUDEFILES:=*.h

OBJ =	(addsuffix .o, $(basename $(wildcard *.c)))

ccc_files = ${OBJ}

print2:
	echo "$(ccc_files)"

export ccc_files

#sources:${OBJ}
#	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}
#%.o: %.c
#	${CC} $(CFLAGS) $(INCLUDES) -o $@ $<

#hello_includes:
#	${CP} ${INCLUDEFILES} ${INCLUDEDIR}

#.c:
#	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

#.c.o:
#	$(CC) $(CFLAGS) -c $<
#.PHONY: clean
#clean:
#	-rm -f *.o core *.core


#lol your good if you need the terminal or whatever thats fine lmk

#.c:
#	$(CC) $(CFLAGS) $@.c $(LDFLAGS) -o $@

#//.c.o:
#//   $(CC) $(CFLAGS) -c $<

#//alternatively

#//.cpp.o:
#//   $(CC) $(CFLAGS) -c $*.cpp
