VPATH=src/ src/Thread src/Config src/Thread/Blocking src/Document src/Document/Handler
CC=/usr/bin/g++
PROGRAM=mp3sort
LDLIBS=-lboost_program_options -lboost_thread-mt -lboost_filesystem -lboost_system -ldl
LDHANDLERLIBS=-lboost_filesystem

all: lib mp3sort

mp3sort: src/mp3sort.cpp Options.o Queue.h Dispatcher.o Default.o Handler.h
	$(CC) -fPIC -Llib/ -o $(PROGRAM) src/mp3sort.cpp Options.o  Dispatcher.o Default.o Handler.o $(LDLIBS) 

lib: MP3.o Handler.o
	rm -rf lib && mkdir lib
	$(CC) -fPIC -c src/Document/Handler/*cpp 
	$(CC) -fPIC -c src/Document/Handler.cpp
	$(CC) -o lib/libhandler-mp3.so.1 -shared MP3.o Handler.o
	$(CC) -o lib/libhandler-ogg.so.1 -shared Ogg.o Handler.o
	
	ln -sf libhandler-mp3.so.1 lib/libhandler-mp3.so
	ln -sf libhandler-ogg.so.1 lib/libhandler-ogg.so

Options.o: Options.h Options.cpp
	$(CC) -c src/Config/Options.cpp

Dispatcher.o: Dispatcher.cpp Default.o
	$(CC) -c src/Document/Dispatcher.cpp src/Document/Handler/Default.cpp src/Document/Handler.cpp

Default.o: 
	$(CC) -c src/Document/Handler/Default.cpp


clean:
	rm -f *.o

proper:
	rm -f *.o $(PROGRAM)
	rm -rf lib
