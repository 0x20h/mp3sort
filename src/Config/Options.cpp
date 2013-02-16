#include "Options.h"
#include <iostream>
#include <fstream>
#include <boost/exception/all.hpp>
#define DEFAULT_CONF_RC "~/.mp3sortrc"

using namespace Config;
using namespace std;
namespace po = boost::program_options;

Options::Options(int argc, char * argv[]) 
{
	po::options_description o_general("General");
	// Declare visible options.
	o_general.add_options()
	    ("help,h", "produce help message")
		("verbose,v", "be more verbose")
  		("force-fingerprint,f", "force generation and querying fingerprint data (e.g. wrong ID3v2 tags)")
		("metadata-only", "only fetch metadata, no sorting.")
		("version", "show version information");
	
	po::options_description o_sort("Sorting");
	o_sort.add_options()
  		("move,m", "delete files from src after sorting")
  		("src,s", po::value<string>(), "read files from source directory")
  		("dst,d", po::value<string>()->default_value("sorted_music/%i/%a/%t_%s"), "Destination directory.\n"
		 	"The following placeholders will be used as replacement parameters:"
			"\n\n"
			"\t%i: Interpret\n"
			"\t%a: Album\n"
			"\t%s: Song Title\n"
			"\t%t: Track Number\n"
		);
	this->o_visible = new po::options_description("Usage: mp3sort options [input] [output]\n\nOptions");
	this->o_visible->add(o_general).add(o_sort);
	
	// declare config file options
	po::options_description o_config("Config file options");
	o_config.add_options()
		// @TODO ()
		;
	this->o_all = new po::options_description("Available Options");
	this->o_all->add(o_general).add(o_sort); // .add(o_config);

	// src & dst dir may be provided as positional arguments
	po::positional_options_description pos;
	pos.add("src", 1);
	pos.add("dst", 2);

	po::store(po::command_line_parser(argc, argv).options(*this->o_visible).positional(pos).run(), this->v_map);
	
	// now merge with config file
	const char* filename = this->hasOption("config") ? this->getOption("config").as<string>().c_str() : DEFAULT_CONF_RC;
	ifstream ifile(filename);

	if (ifile) {
		po::store(po::parse_config_file(ifile, *this->o_all), this->v_map);
	}

	// notify callbacks
	po::notify(this->v_map);
}

const po::options_description Options::getDescription() {
	return *this->o_visible;
}

const bool Options::hasOption(string name) {
	return (bool) this->v_map.count(name);
}

const po::variable_value& Options::getOption(const string name) {
	return this->v_map[name];
}
