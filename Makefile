VPATH=src/ src/Thread src/Config src/Thread/Blocking src/Document src/Document/Handler src/fpclient
CC=/usr/bin/g++
PROGRAM=mp3sort
OBJECTS=MP3.o Handler.o MP3_Source.o HTTPClient.o Default.o Options.o Dispatcher.o
LDLIBS=-lboost_program_options -lboost_thread-mt -lboost_filesystem -lboost_system -lfplib -lfftw3f -lmad -lsamplerate -lcurl

all: mp3sort

mp3sort: src/mp3sort.cpp $(OBJECTS)
	$(CC) -fPIC -Llib/ -o $(PROGRAM) src/mp3sort.cpp $(OBJECTS) $(LDLIBS)

MP3.o: src/Document/Handler/MP3.h src/Document/Handler/MP3.cpp src/fpclient/MP3_Source.h Handler.o
	$(CC) -c src/Document/Handler/MP3.cpp

MP3_Source.o:
	$(CC) -c src/fpclient/MP3_Source.cpp

HTTPClient.o:
	$(CC) -c src/fpclient/HTTPClient.cpp

Handler.o:
	$(CC) -c src/Document/Handler.cpp

clean:
	rm -f *.o

proper:
	rm -f *.o $(PROGRAM)
	rm -rf lib
