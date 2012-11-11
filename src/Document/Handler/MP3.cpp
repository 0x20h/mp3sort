#include "../../fpclient/MP3_Source.h"
#include "../../fpclient/HTTPClient.h"
#include "../../../deps/tinyxml2/tinyxml2.h"
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

const char FP_SERVER_NAME[] = "ws.audioscrobbler.com/fingerprint/query/";
const char METADATA_SERVER_NAME[] = "http://ws.audioscrobbler.com/2.0/";
const char PUBLIC_CLIENT_NAME[]	= "mp3sort 0.4";
const char HTTP_POST_DATA_NAME[] = "fpdata";
const char LASTFM_API_KEY[] = "a2a3170f5858416809be644bb6ba5e27"; 

using namespace Document;
using namespace tinyxml2;

Metadata MP3::getMetadata(const std::string& filename) {
	Metadata meta;
	meta.filename = filename;

	// read from id3v2
	if (readID3v2(filename, meta) && meta.complete()) {
		return meta;
	}

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
		std::cout << "fp created " << std::endl;

		// search the fingerprint	
		std::string fp_response = client.postRawObj(
			FP_SERVER_NAME,
			params,
			fpData.first,
			fpData.second,
			HTTP_POST_DATA_NAME,
			false
		);

		if (fp_response.find("FOUND")) {
			int fpid;
			istringstream iss(fp_response);
			iss >> fpid;

			ostringstream oss;
			oss << METADATA_SERVER_NAME 
					<< "?method=track.getfingerprintmetadata"
					<< "&fingerprintid=" << fpid
					<< "&api_key=" << LASTFM_API_KEY;
			std::cout << ">>" << oss.str() << std::endl;
			// Get Fingerprint Metadata
			std::string metadata_response = client.get(oss.str());
			
//		std::cout << metadata_response << std::endl;	
			XMLDocument* meta_doc = new XMLDocument();
			meta_doc->Parse(metadata_response.c_str());
			const char* mbid = meta_doc
				->FirstChildElement("lfm")
				->FirstChildElement("tracks")
				->FirstChildElement("track")
				->FirstChildElement("mbid")
				->GetText();

			if (mbid == NULL) {
				std::cout << "NO mbid " << mbid << std::endl;
				return meta;
			}

			ostringstream oss2;
			oss2 << METADATA_SERVER_NAME 
					<< "?method=track.getInfo"
					<< "&mbid=" << mbid
					<< "&api_key=" << LASTFM_API_KEY;

			// Get Track info
			std::string track_info_response = client.get(oss2.str());
			
			std::cout << "-----" << std::endl;	
			std::cout << track_info_response << std::endl;	
			
			XMLDocument* doc = new XMLDocument();
			doc->Parse(track_info_response.c_str());
			XMLElement* track = doc->FirstChildElement("lfm")->FirstChildElement("track");

			if (track != NULL) {
				meta.interpret = track->FirstChildElement("artist")->FirstChildElement("name")->GetText();
				meta.title = track->FirstChildElement("name")->GetText();
				// TODO: Fetch album info, release year
				meta.album = track->FirstChildElement("album")->FirstChildElement("title")->GetText();	
			}
		} else {
			std::cout << "Unknown: " << fp_response << std::endl;
		}
	} catch(const std::exception &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return meta;
	}

	return meta;
}

void MP3::storeMetadata(const std::string& filename, const Metadata& meta) {

	std::cout << "storing metadata for " << filename << std::endl;
	//
}

bool MP3::readID3v2(const std::string& filename, Metadata& meta) {
	try {
		TagLib::MPEG::File f(filename.c_str());
		
		if (f.isValid() && f.tag()) {
			TagLib::Tag* pTag = f.tag();
			meta.interpret = pTag->artist().to8Bit(true);
			meta.album = pTag->album().to8Bit(true);
			meta.title = pTag->title().to8Bit(true);
			meta.album = pTag->album().to8Bit(true);
			meta.genre = pTag->genre().to8Bit(true);
			meta.track_no = pTag->track();
			meta.year = pTag->year();
			return meta.interpret != "" && meta.album != "" && meta.title != "" && meta.track_no > 0;
		}
	} catch (const std::exception&) {}
	
	return false;
}
