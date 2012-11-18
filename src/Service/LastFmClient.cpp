#include "LastFmClient.h"

#include <iostream>
#include <sstream>

const char FP_SERVER_NAME[] = "ws.audioscrobbler.com/fingerprint/query/";
const char METADATA_SERVER_NAME[] = "http://ws.audioscrobbler.com/2.0/";
const char PUBLIC_CLIENT_NAME[]	= "mp3sort 0.4";
const char HTTP_POST_DATA_NAME[] = "fpdata";
const char LASTFM_API_KEY[] = "a2a3170f5858416809be644bb6ba5e27"; 

using namespace tinyxml2;
using namespace Service;
using namespace std;

LastFmClient::LastFmClient() {
	// get HTTPClient instance
	HTTPClient client;
}

// -----------------------------------------------------------------------------

LastFmClient::~LastFmClient() {
}

XMLDocument* LastFmClient::trackGetInfo(string mbid) {
	ostringstream oss;
	oss << METADATA_SERVER_NAME 
			<< "?method=track.getInfo"
			<< "&mbid=" << mbid
			<< "&api_key=" << LASTFM_API_KEY;
	
	string response = client.get(oss.str());
	XMLDocument* doc = new XMLDocument();
	doc->Parse(response.c_str());
    return doc;	
}

XMLDocument* LastFmClient::albumGetInfo(string mbid) {
	ostringstream oss;
	oss << METADATA_SERVER_NAME 
			<< "?method=album.getInfo"
			<< "&mbid=" << mbid
			<< "&api_key=" << LASTFM_API_KEY;
	
	string response = client.get(oss.str());
	XMLDocument* doc = new XMLDocument();
	doc->Parse(response.c_str());
    return doc;
}

XMLDocument* LastFmClient::albumGetInfo(string artist, string album) {
	ostringstream oss;
	oss << METADATA_SERVER_NAME << "?" 
			<< "method=album.getInfo"
			<< "&album=" << client.encode(album)
			<< "&artist=" << client.encode(artist)
			<< "&api_key=" << LASTFM_API_KEY;
	
	string response = client.get(oss.str());
	XMLDocument* doc = new XMLDocument();
	doc->Parse(response.c_str());
    return doc;
}

string LastFmClient::artistGetInfo(string mbid) {
}

XMLDocument* LastFmClient::fingerprintGetMetadata(int fpid) {
	ostringstream oss;
	oss << METADATA_SERVER_NAME 
			<< "?method=track.getfingerprintmetadata"
			<< "&fingerprintid=" << fpid
			<< "&api_key=" << LASTFM_API_KEY;
	
	string response = client.get(oss.str());
	XMLDocument* doc = new XMLDocument();
	doc->Parse(response.c_str());
    return doc;	
}

int LastFmClient::getFingerprint(pair<const char*, size_t> fpData, map<string, string> params) {
	int fpid;
	// search the fingerprint	
	string fp_response = client.postRawObj(
		FP_SERVER_NAME,
		params,
		fpData.first,
		fpData.second,
		HTTP_POST_DATA_NAME,
		false
	);

	if (fp_response.find("FOUND")) {
		istringstream iss(fp_response);
		iss >> fpid;
	}

	return fpid;
}

void LastFmClient::print(XMLDocument* doc) {
    XMLPrinter p;
    doc->Print(&p);
    cout << p.CStr() << endl; 
}


void LastFmClient::getMetadata(int fpid, Document::Metadata* meta) {
    XMLDocument* fpMeta = fingerprintGetMetadata(fpid);
    // clear match ?
	XMLElement* fpMeta_track = fpMeta->FirstChildElement("lfm")->FirstChildElement("tracks")->FirstChildElement("track");
    const char* rank = fpMeta_track->Attribute("rank");
	if (rank == NULL) {
        return;
    }

    // confident match ?
    if (atoi(rank) == 1) {
		// try to extract song mbid
		const char* trackId = fpMeta_track->FirstChildElement("mbid")->GetText();
		meta->interpret = fpMeta_track->FirstChildElement("artist")->FirstChildElement("name")->GetText();
		meta->title = fpMeta_track->FirstChildElement("name")->GetText();

		// fetch song info
		if (trackId != NULL) {
			XMLDocument* trackInfo = trackGetInfo(trackId);
			XMLElement* trackInfo_track = trackInfo->FirstChildElement("lfm")->FirstChildElement("track");
			// fetch album info
			XMLElement* trackInfo_album = trackInfo_track->FirstChildElement("album");
			if (trackInfo_album != NULL) {
				meta->album = trackInfo_album->FirstChildElement("title")->GetText();
				meta->track_no = trackInfo_album->IntAttribute("position");
				XMLDocument* albumInfo = albumGetInfo(meta->interpret, meta->album);
				const char* release = albumInfo->FirstChildElement("lfm")->FirstChildElement("album")->FirstChildElement("releasedate")->GetText();
				if (release != NULL && release != "") {
					meta->year = atoi(release);
				}
			}
		}
	}
}
