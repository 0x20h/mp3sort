#include "../../../deps/0x20h_Fingerprinter/src/fpclient/MP3_Source.h"
#include "../../Service/LastFmClient.h"
#include "MP3.h"

#include <iostream>
#include <fstream>
#include <boost/thread/mutex.hpp>
#include <taglib/mpegfile.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/id3v2tag.h>
#include <fplib/FingerprintExtractor.h>


static boost::mutex fpe_mutex;
static boost::mutex taglib_mutex;

using namespace Document;

Metadata MP3::getMetadata(const std::string& filename) {
	Metadata meta;
	meta.filename = filename;
	Service::LastFmClient client(options);
	
	// read from id3v2
	if (readID3v2(filename, meta) && meta.complete() && !options->hasOption("force-fingerprint")) {
		return meta;
	}

	std::map<std::string, std::string> params;
	int duration, srate, bitrate, nchannels;
	const size_t pcmBufSize = 131072;
	short *pPCMBuffer = new short[pcmBufSize];

	MP3_Source mp3;
	mp3.getInfo(filename, duration, srate, bitrate, nchannels);
	mp3.init(filename);
	mp3.skipSilence();

	try {
		fpe_mutex.lock(); // due to fftwf_plan_*
		fingerprint::FingerprintExtractor fextr;
		fpe_mutex.unlock();
		fextr.initForQuery(srate, nchannels, duration);
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
		int fpid = client.getFingerprint(fpData, params);
		client.getMetadata(fpid, &meta);
	} catch(const std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return meta;
	}

	return meta;
}

void MP3::storeMetadata(const std::string& filename, const Metadata& meta) {
	std::cout << "storing metadata for " << filename << std::endl;
}

bool MP3::readID3v2(const std::string& filename, Metadata& meta) {
	// After reading http://mail.kde.org/pipermail/taglib-devel/2008-March/000902.html
	// adding a big lock seems reasonable
	try {
		taglib_mutex.lock();	
		TagLib::MPEG::File f(filename.c_str());
		
		if (f.isValid() && f.tag()) {
			TagLib::Tag* pTag = f.tag();
			meta.interpret = pTag->artist().isNull() ? "" : pTag->artist().to8Bit(true);
			meta.album = pTag->album().isNull() ? "" : pTag->album().to8Bit(true);
			meta.title = pTag->title().isNull() ? "" : pTag->title().to8Bit(true);
			meta.genre = pTag->genre().isNull() ? "" : pTag->genre().to8Bit(true);
			meta.track_no = pTag->track();
			meta.year = pTag->year();
			taglib_mutex.unlock();
			return meta.complete();
		}
	} catch (const std::exception&) {
		taglib_mutex.unlock();
		// ...
	}
	
	return false;
}
