#include <boost/program_options.hpp>

#ifndef CONFIG_OPTIONS_H
#define CONFIG_OPTIONS_H

namespace po = boost::program_options;

namespace Config
{
	class Options {
		public:
			Options(int argc, char * argv[]);
			bool hasOption(std::string name);
			const po::variable_value& getOption(std::string name);
			const po::options_description getDescription();
		private:
			po::variables_map v_map;
			po::options_description * o_visible;
			po::options_description * o_all;
	};
}

#endif // CONFIG_OPTIONS_H
