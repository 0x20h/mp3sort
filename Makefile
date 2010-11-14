VPATH=src/ src/Document src/Struct src/Thread src/Config 
CC=/usr/bin/g++
PROGRAM=mp3sort
LDLIBS=-lboost_program_options
mp3sort: src/mp3sort.cpp Options.o 
	$(CC) -o $(PROGRAM) src/mp3sort.cpp Options.o  $(LDLIBS)

Options.o: Options.h Options.cpp
	$(CC) -c src/Config/Options.cpp

clean:
	rm -f *.o

proper:
	rm -f *.o $(PROGRAM)
