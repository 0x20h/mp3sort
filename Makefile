VPATH=src/ src/Thread src/Config src/Thread/Blocking src/Document src/Document/Handler
CC=/usr/bin/g++
CXXFLAGS=
PROGRAM=mp3sort
FPOBJ=deps/0x20h_Fingerprinter/CMakeFiles/lastfm-fpclient.dir/src/fpclient/HTTPClient.cpp.o deps/0x20h_Fingerprinter/CMakeFiles/lastfm-fpclient.dir/src/fpclient/MP3_Source.cpp.o
OBJECTS=LastFmClient.o MP3.o Handler.o Default.o Options.o Dispatcher.o Worker.o Metadata.o tinyxml2.o $(FPOBJ) 
LDLIBS=-lboost_program_options -lboost_thread-mt -lboost_filesystem -lboost_system -lfftw3f -lmad -lsamplerate -lcurl -ltag
all: tinyxml fingerprinter mp3sort

mp3sort: src/mp3sort.cpp $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $(PROGRAM) src/mp3sort.cpp $(OBJECTS) deps/0x20h_Fingerprinter/lib/libfplib.a $(LDLIBS)

fingerprinter:
	cd deps/0x20h_Fingerprinter/ && cmake . && make

tinyxml:
	$(CC) -c deps/tinyxml2/tinyxml2.cpp

MP3.o: src/Document/Handler/MP3.h src/Document/Handler/MP3.cpp Handler.o LastFmClient.o
	$(CC) -Ideps/0x20h_Fingerprinter/include -c src/Document/Handler/MP3.cpp 

LastFmClient.o: src/Service/LastFmClient.h src/Service/LastFmClient.cpp
	$(CC) -c src/Service/LastFmClient.cpp 


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
	cd deps/0x20h_Fingerprinter && make clean

proper:
	rm -f *.o $(PROGRAM)
