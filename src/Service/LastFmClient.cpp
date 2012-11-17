#include "LastFmClient.h"

#include <iostream>
#include <sstream>

const char FP_SERVER_NAME[] = "ws.audioscrobbler.com/fingerprint/query/";
const char METADATA_SERVER_NAME[] = "http://ws.audioscrobbler.com/2.0/";
const char PUBLIC_CLIENT_NAME[]	= "mp3sort 0.4";
const char HTTP_POST_DATA_NAME[] = "fpdata";
const char LASTFM_API_KEY[] = "a2a3170f5858416809be644bb6ba5e27"; 

using namespace Service;
using namespace std;

LastFmClient::LastFmClient() {
	// get HTTPClient instance
	HTTPClient client;
}

// -----------------------------------------------------------------------------

LastFmClient::~LastFmClient() {
}

string LastFmClient::trackGetInfo(string mbid) {
}

string LastFmClient::albumGetInfo(string mbid) {
}

string LastFmClient::artistGetInfo(string mbid) {
}

string LastFmClient::fingerprintGetMetadata(int fpid) {
		ostringstream oss;
	oss << METADATA_SERVER_NAME 
			<< "?method=track.getfingerprintmetadata"
			<< "&fingerprintid=" << fpid
			<< "&api_key=" << LASTFM_API_KEY;
	
	cout << ">>" << oss.str() << endl;
	// Get Fingerprint Metadata
	string metadata_response = client.get(oss.str());
	return metadata_response;	
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


void LastFmClient::getMetadata(int fpid, Document::Metadata* meta) {
}
