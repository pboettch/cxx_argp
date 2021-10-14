#include <cxx_argp_parser.h>

#include <iostream>

int main(int argc, char *argv[]) {

	std::pair<std::ifstream, std::string> file{std::ifstream{"/etc/group"}, "/etc/group"};

    cxx_argp::parser parser;

    parser.add_option({"conf", 'c', "<file_path>", 0, "a configuration file path"}, file);

    if (parser.parse(argc, argv)) {
		std::cerr << "parsing OK\n";
	} else {
		std::cerr << "there was an error - exiting\n";
		return 1;
	}

	if (file.second != "/etc/passwd") {
		std::cerr << "file argument not overriden\n";

		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}
