VPATH=src/ src/Struct src/Thread src/Config src/Thread/Blocking src/Document src/Document
CC=/usr/bin/g++
PROGRAM=mp3sort
LDLIBS=-lboost_program_options -lboost_thread-mt -lboost_filesystem -ldl
LDHANDLERLIBS=-lboost_filesystem

all: lib mp3sort

mp3sort: src/mp3sort.cpp Options.o Queue.h
	$(CC) -fPIC -Llib/ -o $(PROGRAM) src/mp3sort.cpp Options.o $(LDLIBS) 

lib: src/Document/Handler/* src/Document/Handler.h
	rm -rf lib && mkdir lib
	$(CC) -c src/Document/Handler/*cpp 
	$(CC) -o lib/libhandler-mp3.so.1 -shared MP3.o
	$(CC) -o lib/libhandler-ogg.so.1 -shared Ogg.o
	ldconfig -v -n lib
	ln -sf libhandler-mp3.so.1 lib/libhandler-mp3.so
	ln -sf libhandler-ogg.so.1 lib/libhandler-ogg.so

Options.o: Options.h Options.cpp
	$(CC) -c src/Config/Options.cpp

clean:
	rm -f *.o

proper:
	rm -f *.o $(PROGRAM)
	rm -rf lib
