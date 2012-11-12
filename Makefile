VPATH=src/ src/Thread src/Config src/Thread/Blocking src/Document src/Document/Handler src/fpclient
CC=/usr/bin/g++
PROGRAM=mp3sort
OBJECTS=MP3.o Handler.o MP3_Source.o HTTPClient.o Default.o Options.o Dispatcher.o Worker.o Metadata.o tinyxml2.o 
LDLIBS=-lboost_program_options -lboost_thread-mt -lboost_filesystem -lboost_system -lfftw3f -lmad -lsamplerate -lcurl -ltag
all: tinyxml fingerprinter mp3sort

mp3sort: src/mp3sort.cpp $(OBJECTS)
	$(CC) -o $(PROGRAM) src/mp3sort.cpp $(OBJECTS) deps/Fingerprinter/lib/libfplib.a $(LDLIBS)

fingerprinter:
	cd deps/Fingerprinter/ && cmake . && make

tinyxml:
	$(CC) -c deps/tinyxml2/tinyxml2.cpp

MP3.o: src/Document/Handler/MP3.h src/Document/Handler/MP3.cpp src/fpclient/MP3_Source.h Handler.o 
	$(CC) -Ideps/Fingerprinter/include -c src/Document/Handler/MP3.cpp 

MP3_Source.o:
	$(CC) -c src/fpclient/MP3_Source.cpp

HTTPClient.o:
	$(CC) -c src/fpclient/HTTPClient.cpp

Handler.o:
	$(CC) -c src/Document/Handler.cpp

Dispatcher.o: Dispatcher.h Dispatcher.cpp
	$(CC) -c src/Document/Dispatcher.cpp

Options.o: Options.cpp Options.h
	$(CC) -c src/Config/Options.cpp

Worker.o: Worker.h Worker.cpp
	$(CC) -c src/Document/Worker.cpp

Metadata.o: Metadata.h Metadata.cpp
	$(CC) -c src/Document/Metadata.cpp


clean:
	rm -f *.o
	cd deps/tinyxml2 && make clean
	cd deps/Fingerprinter && make clean

proper:
	rm -f *.o $(PROGRAM)
