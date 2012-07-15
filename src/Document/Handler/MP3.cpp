#include "MP3.h"
#include <iostream>
#include <fstream>
#include <boost/thread/mutex.hpp>
static boost::mutex fpe_mutex;

const char FP_SERVER_NAME[] = "ws.audioscrobbler.com/fingerprint/query/";
const char METADATA_SERVER_NAME[] = "http://ws.audioscrobbler.com/2.0/";
const char PUBLIC_CLIENT_NAME[]	= "fp client 1.6";
const char HTTP_POST_DATA_NAME[] = "fpdata";
const char LASTFM_API_KEY[] = "2bfed60da64b96c16ea77adbf5fe1a82";

using namespace Document;

MP3::MP3() {}
MP3::~MP3() {}

Metadata MP3::process(const std::string filename) {
	Metadata meta;
	HTTPClient client;
	std::map<std::string, std::string> params;
	int duration, srate, bitrate, nchannels;
	const size_t pcmBufSize = 131072;
	short *pPCMBuffer = new short[pcmBufSize];

	MP3_Source mp3;
	mp3.getInfo(filename, duration, srate, bitrate, nchannels);
	mp3.init(filename);
	mp3.skipSilence();
	try {
		fpe_mutex.lock();
		fingerprint::FingerprintExtractor fextr;
		fpe_mutex.unlock();
		fextr.initForQuery(srate, nchannels, duration);
		cout << "info: " << duration << "," << srate << "," << bitrate << "," << nchannels << std::endl;
		mp3.skip(static_cast<int>(fextr.getToSkipMs()));
		fextr.process(0, static_cast<size_t>(srate * nchannels * (fextr.getToSkipMs() / 1000.0)));

		while(true) {
			size_t r = mp3.updateBuffer(pPCMBuffer, pcmBufSize);

			if (!r) {
				std::cerr << "no input data" << std::endl;
			}

			if (fextr.process(pPCMBuffer, r, mp3.eof())) {
				break;
			}
		}

		params["username"] = "Ox2Oh";
		params["duration"] = boost::lexical_cast<std::string>(duration);
		params["samplerate"] = boost::lexical_cast<std::string>(srate);

		pair<const char*, size_t> fpData = fextr.getFingerprint();

/*		std::string s = client.postRawObj(
			FP_SERVER_NAME,
			params,
			fpData.first,
			fpData.second,
			HTTP_POST_DATA_NAME,
			false
		);
*/
		std::cout << "FP: " << fpData.second << std::endl;
	} catch(const std::exception &e) {
		std::cerr << "Exception: " <<e.what() << std::endl;
		return meta;
	}

	std::cout << "finished " << filename << std::endl;
	return meta;
}

std::string MP3::getDescription() {
	return "Mp3 Handler";
}

extern "C" {
	Handler* factory() { return new MP3; } 
}
