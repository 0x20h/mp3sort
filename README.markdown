mp3sort
-------
mp3sort helps sort your mp3 collection using information retrieved from the fingerprinting service provided by [last.fm API](http://www.lastfm.de/api/show/track.getFingerprintMetadata).
This project is still in early development, no warranties.

``` sh
mp3sort /from/src/directory /to/dest/directory/%i/%a/%n-%t [-move]
```
- %i: Interpret
- %a: Album
- %n: Track number
- %t: Track name

mp3sort 
- reads all music files from src directory, get metadata for each song and sort (according to dest-pattern) in dest directory
- deletes in src directory if -move
- retrieves metadata from lastfm using lastfm's fingerprinting library if ID3v2 tag is missing
- updates metadata as ID3v2 tag

Build 
-----

For [Fingerprinter](https://github.com/lastfm/Fingerprinter):
- libmad
- fftw
- libsamplerate
- libtag
- libcurl
- cmake

Boost libraries:
- boost-thread
- boost-program-options
- boost-filesystem


``` sh
git clone https://github.com/0x20h/mp3sort.git
cd mp3sort
git submodule update --init

sudo apt-get install cmake libmad0-dev libfftw3-dev libsamplerate-dev libtag1-dev libboost-dev libboost-thread-dev libboost-filesystem-dev libcurl4-dev libboost-program-options-dev
make
```

Misc
----

for music classification see (fingerprinter is based on this work)
http://www.cs.cmu.edu/~yke/musicretrieval/cvpr2005-mr.pdf

License
-------

[GPL v3](http://www.gnu.org/licenses/gpl.txt)
