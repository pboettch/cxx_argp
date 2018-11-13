#include <cxx_argp_parser.h>
#include <iostream>

int main(int argc, char *argv[])
{
	// initializers are default values for omitted options
	std::string host = "127.0.0.1";

	// create parser object,
	cxx_argp::parser parser;

	// add option and associate to simple variables, the class creates a
	// conversion function (if the type is supported) which does a basic check
	// and fills the variable
	// Note: the first argument is the real 'struct argp_option'
	parser.add_option({nullptr, 'h', "host-address", 0, "IP address of host"},
	                  host);

	if (parser.parse(argc, argv)) {
		std::cerr << "parsing OK\n";
	} else {
		std::cerr << "there was an error - exiting\n";
		return 1;
	}

	std::cerr << "hostname " << host << "\n";

	// here, do more checks on the values passed to the program

	return 0;
}
