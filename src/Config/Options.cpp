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
		("version", "show version information")
		("config,c", po::value<string>()->default_value("~/.mp3sortrc"), "read configuration from file");

	
	po::options_description o_sort("Sorting");
	o_sort.add_options()
  		("move,m", "delete files from src after sorting")
  		("src,s", po::value<string>(), "read files from source directory")
  		("dst,d", po::value<string>(), "destination directory where to put sorted files");

	this->o_visible = new po::options_description("Available Options");
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

	try {
		po::store(po::command_line_parser(argc, argv).options(*this->o_visible).positional(pos).run(), this->v_map);
	
		// now merge with config file
		const char* filename = this->hasOption("config") ? this->getOption("config").as<string>().c_str() : DEFAULT_CONF_RC;
		ifstream ifile(filename);
		if (ifile) {
			try {
				po::store(po::parse_config_file(ifile, *this->o_all), this->v_map);
			} catch (exception &e) {
				cerr << "Error parsing " << filename << ". " << e.what() << endl;	
			}
		} else {
			cerr << "config file " << filename  << " does not exist. skipping..." << endl;
		}
	} catch (exception &e) {
		cerr << "Error: " << e.what() << endl;
	}
	
	// notify callbacks
	po::notify(this->v_map);
}

const po::options_description Options::getDescription() {
	return *this->o_visible;
}

bool Options::hasOption(string name)
{
	return (bool) this->v_map.count(name);
}

const po::variable_value& Options::getOption(string name)
{
	return this->v_map[name];
}
