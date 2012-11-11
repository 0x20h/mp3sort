mp3sort Project
-----------------
mp3sort helps sort your mp3 collection using information retrieved from the fingerprinting service provided by last.fm.
This project is still in early development.

mp3sort /from/src/directory /to/dest/directory/%i/%a-%y/%n-%t [-move]

- read all music files from src directory, get metadata for each song and sort (according to config) in dest directory
- delete in src directory if -move
- metadata retrieval from lastfm using lastfm's fingerprinting library
- set metadata as ID3 tag
- config in .mp3sortrc

Build Dependencies
---------------------------

For Fingerprinter (https://github.com/lastfm/Fingerprinter):
- libmad
- fftw
- libsamplerate
- libtag
- libcurl
- cmake

Boost libraries:
- boost-thread
- boost-filesystem

```
sudo apt-get install cmake libmad0-dev libfftw3-dev libsamplerate-dev libtag1-dev libboost-dev libboost-thread-dev libboost-filesystem-dev libcurl4-dev libboost-program-options-dev
```

Milestones
----------------------------

0.1
- specification of Config 
- ability to call program with options, options are set correctly.
- ability to read default options from config file

0.2
- specification of Document and Document::Handler
- master thread reads directory and creates a handler for each file, calls Handler::process() in a thread environment
- Handler::process(filename) says "hello, would handle filename", sleep 1 second, then exit

0.3
- mp3 handler opens mp3file, reads sample data and delivers to fingerprint lib
- returned results are used to move file to correct folder

Document
-------------------

Document::Handler Document::create_handler_from_file(std::string filename) 
creates a file handler according to file content.

void Handler::process() 
determines the path where this file is sorted into and moves file.

We work on Documents and Document::Handlers.
Document stores information on
- the file storage (e.g. pathinformation)
- mimetype
- date stuff
- mimetype specific metainformation (from that we can extract the sorted destination)
- tags ???


Document::Handler knows how to...
- determine destination path from the document information (e.g. metadata, fetch info from lastfm for .mp3)
- Document::Handler must be implemented by all filetype handlers

Misc
----

for music classification see (fingerprinter is based on this work)
http://www.cs.cmu.edu/~yke/musicretrieval/cvpr2005-mr.pdf

License
-------

[GPL v3](http://www.gnu.org/licenses/gpl.txt)
