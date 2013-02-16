mp3sort Project
-----------------
mp3sort helps sort your mp3 collection using information retrieved from the fingerprinting service provided by last.fm.
This project is still in early development, no warranties.

mp3sort /from/src/directory /to/dest/directory/%i/%a-%y/%n-%t [-move]

- read all music files from src directory, get metadata for each song and sort (according to dest-pattern) in dest directory
- delete in src directory if -move
- metadata retrieval from lastfm using lastfm's fingerprinting library if ID3v2 tag missing
- set metadata as ID3 tag

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

Misc
----

for music classification see (fingerprinter is based on this work)
http://www.cs.cmu.edu/~yke/musicretrieval/cvpr2005-mr.pdf

License
-------

[GPL v3](http://www.gnu.org/licenses/gpl.txt)
