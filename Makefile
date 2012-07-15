VPATH=src/ src/Thread src/Config src/Thread/Blocking src/Document src/Document/Handler src/fpclient
CC=/usr/bin/g++
PROGRAM=mp3sort
LDLIBS=-lboost_program_options -lboost_thread-mt -lboost_filesystem -lboost_system -ldl 

all: lib mp3sort

mp3sort: src/mp3sort.cpp Options.o Queue.h Dispatcher.o Default.o Handler.h
	$(CC) -fPIC -Llib/ -o $(PROGRAM) src/mp3sort.cpp Options.o  Dispatcher.o Default.o Handler.o $(LDLIBS) 

lib: MP3.o Handler.o MP3_Source.o HTTPClient.o
	rm -rf lib/libhandler-*
	$(CC) -fPIC -o lib/libhandler-mp3.so.1 -shared MP3.o Handler.o MP3_Source.o HTTPClient.o -lfplib -lfftw3f -lmad -lsamplerate -lcurl -lboost_thread-mt
	ln -sf libhandler-mp3.so.1 lib/libhandler-mp3.so

MP3.o: src/Document/Handler/MP3.h src/Document/Handler/MP3.cpp src/fpclient/MP3_Source.h
	$(CC) -fPIC -IFingerprinter/include -c src/Document/Handler/MP3.cpp

MP3_Source.o:
	$(CC) -fPIC -c src/fpclient/MP3_Source.cpp

HTTPClient.o:
	$(CC) -fPIC -c src/fpclient/HTTPClient.cpp

Handler.o:
	$(CC) -fPIC -c src/Document/Handler.cpp
clean:
	rm -f *.o

proper:
	rm -f *.o $(PROGRAM)
	rm -rf lib
