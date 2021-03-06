#include <iostream>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include "Config/Options.h"
#include "Document/Handler.h"
#include "Document/Dispatcher.h"
#define VERSION "0.3"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[]) {
	try {
		Config::Options options = Config::Options(argc, argv);
		
		if (argc < 2 || options.hasOption("help")) {
			std::cout << options.getDescription() << std::endl;
			return 0;
		}
	
		if (options.hasOption("version")) {
			std::cout << "Version: " << VERSION << std::endl;
			return 0;
		}

	
		if (!options.hasOption("src")) {
			std::cerr << "ERROR: please specify source directory" << std::endl;
			std::cout << options.getDescription() << std::endl;
			return 1;
		}

		Document::Dispatcher dispatcher(options);
		dispatcher.init();
		string srcPath = options.getOption("src").as<string>();
		path src(srcPath);

		try {
			if (exists(src)) {
				if (is_directory(src)) {
					for(recursive_directory_iterator end, dir(src); dir != end; ++dir) {
						string f = path(*dir).string();
						if(is_regular_file(f)) {
							// only add files
							dispatcher.dispatch(f);
						}
					}
				} else if(is_regular_file(src)) {
					dispatcher.dispatch(src.string());
				}
			}
		} catch(const filesystem_error &e) {
		
		}
	
		dispatcher.join();
	} catch(std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;
}
