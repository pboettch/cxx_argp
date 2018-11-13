#include <cxx_argp_parser.h>

#include <iostream>

#include "test.h"

// creating real argv-strings here
#pragma GCC diagnostic ignored "-Wwrite-strings"

// create a helper of for basic variable-based options
class Main
{
	cxx_argp::parser parser_;

public:
	struct {
		float single = 1.1f;
		double doublef = 2.2;

		uint16_t port = 502;
		int16_t sint = 0;
		std::string host = "127.0.0.1";
		std::string name = "default";

		bool enable = false;

		std::vector<int> vec;

		std::ifstream file;
		std::pair<std::ifstream, std::string> file_and_name;
	} args;

	Main(ssize_t count = 0)
	    : parser_(count)
	{

		// in case of erros, we do not want argp do exit during tests
		parser_.add_flags(ARGP_NO_EXIT);

		parser_.add_option({"float", 's', "single", 0, "floating-point test"}, args.single);
		parser_.add_option({"double", 'd', "double", 0, "floating-point test"}, args.doublef);
		parser_.add_option({"port", 'p', "port", 0, "TCP port the server will listen on"}, args.port);
		parser_.add_option({"host", 'h', "host-address", 0, "IP address of host"}, args.host);
		parser_.add_option({"name", 'n', "name", 0, "a name"}, args.name);
		parser_.add_option({"file", 'f', "filename", 0, "a file"}, args.file);
		parser_.add_option({"enable", 'e', nullptr, 0, "enable"}, args.enable);
		parser_.add_option({"vector", 'V', "list", 0, "list of ints"}, args.vec);
		parser_.add_option({"output", 'o', "filename", 0, "filename and file"}, args.file_and_name);
	}

	bool test(int argc, char *argv[])
	{
		return parser_.parse(argc, argv);
	}

	const std::vector<std::string> &arguments() const { return parser_.arguments(); }
};

TEST(CmdlineArgs, no_args)
{
	char *argv[] = {"program-name"};

	cxx_argp::parser parser;

	ASSERT_EQ(parser.parse(sizeof(argv) / sizeof(argv[0]), argv), true);
}

TEST(CmdlineArgs, one_arg)
{
	char *argv[] = {
	    "program-name",
	    "-n",
	    "default2",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.single, 1.1f);
	EXPECT_EQ(main.args.doublef, 2.2);
	EXPECT_EQ(main.args.port, 502);
	EXPECT_EQ(main.args.sint, 0);
	EXPECT_EQ(main.args.name, "default2");
	EXPECT_EQ(main.args.vec.size(), 0U);
	EXPECT_EQ(main.arguments().size(), 0U);
}

TEST(CmdlineArgs, no_value)
{
	char *argv[] = {
	    "program-name",
	    "-d",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), false);
}

TEST(CmdlineArgs, floating_point_args)
{
	char *argv[] = {
	    "program-name",
	    "-s", "-10.11",
	    "-d", "-10.12",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.single, -10.11f);
	EXPECT_EQ(main.args.doublef, -10.12);
}

TEST(CmdlineArgs, string_args)
{
	char *argv[] = {
	    "program-name",
	    "--name", "long string argument",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.name, "long string argument");
}

TEST(CmdlineArgs, bool_arg)
{
	char *argv[] = {
	    "program-name",
	    "-e",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.enable, true);
}

TEST(CmdlineArgs, file_arg)
{
	char *argv[] = {
	    "program-name",
	    "-f", "/bin/sh",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.file.is_open(), true);
}

TEST(CmdlineArgs, file_and_namearg)
{
	char *argv[] = {
	    "program-name",
	    "-o", "/bin/sh",
	};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.file_and_name.first.is_open(), true);
	EXPECT_EQ(main.args.file_and_name.second, "/bin/sh");
}

TEST(CmdlineArgs, non_existant_file_arg)
{
	char *argv[] = {"program-name",
	                "-f", "/should-not-exist"};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), false);
}

TEST(CmdlineArgs, positional_args)
{
	char *argv[] = {"program-name",
	                "first", "second"};

	Main main(2);

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.arguments()[0], "first");
	EXPECT_EQ(main.arguments()[1], "second");
}

TEST(CmdlineArgs, not_enough_positional_args)
{
	char *argv[] = {"program-name",
	                "first", "second"};

	Main main(3);

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), false);

	EXPECT_EQ(main.arguments()[0], "first");
	EXPECT_EQ(main.arguments()[1], "second");
}

TEST(CmdlineArgs, unlimited_positional_args)
{
	char *argv[] = {"program-name",
	                "first", "second"};

	Main main(-1);

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.arguments()[0], "first");
	EXPECT_EQ(main.arguments()[1], "second");
}

TEST(CmdlineArgs, list_args)
{
	char *argv[] = {"program-name",
	                "-V", "1,2,3,4"};

	Main main;

	ASSERT_EQ(main.test(sizeof(argv) / sizeof(argv[0]), argv), true);

	EXPECT_EQ(main.args.vec.size(), 4U);
	EXPECT_EQ(main.args.vec[0], 1);
	EXPECT_EQ(main.args.vec[3], 4);
}

int main(void)
{
#if 0
	struct {
		float single = 1.1f;
		double doublef = 2.2;

		uint16_t port = 502;
		int16_t sint = 0;
		std::string host = "127.0.0.1";
		std::string name = "default";
		std::ifstream file;

		bool enable = false;

		std::vector<int> vec;

	} args;

	cxx_argp::parser parser(0);

	parser.add_flags(ARGP_NO_EXIT );


	parser.add_option({"float", 's', "single", 0, "floating-point test"}, args.single);
	parser.add_option({"double", 'd', "double", 0, "floating-point test"}, args.doublef);
	parser.add_option({"port", 'p', "port", 0, "TCP port the server will listen on"}, args.port);
	parser.add_option({"host", 'h', "host-address", 0, "IP address of host"}, args.host);
	parser.add_option({"name", 'n', "name", 0, "a name"}, args.name);
	parser.add_option({"file", 'f', "filename", 0, "a file"}, args.file);
	parser.add_option({"enable", 'e', nullptr, 0, "enable"}, args.enable);
	parser.add_option({"vector", 'V', "comma-sep-list", 0, "list of ints"}, args.vec);


	int var;

	parser.add_option({nullptr, 'K', "INT", 0, "an integer"},
	                  [&var](const char *arg) { var = atoi(arg); return true; });

	if (parser.parse(argc, argv)) {
		std::cerr << "parsing OK\n";
	} else
		std::cerr << "parsing failed\n";
#endif

	for (auto &t : tests__)
		t();

	if (result__)
		std::cerr << result__ << " test-condition(s) failed\n";
	else
		std::cerr << "all tests OK\n";

	return result__;
}
