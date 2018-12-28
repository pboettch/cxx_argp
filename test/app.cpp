#include <cxx_argp_application.h>

#include <chrono>
#include <iostream>
#include <thread>

CXX_ARGP_APPLICATION_BOILERPLATE;

class my_app : public cxx_argp::application
{
	struct {
		std::string host;
	} args_;

	bool check_arguments() override
	{
		if (args_.host.size() < 4 ||
		    args_.host.substr(args_.host.size() - 4) != ".org") {
			std::cerr << "only .org-addresses are allowed, " << args_.host << " is not .org.\n";
			return false;
		}

		return true;
	}

	int main()
	{
		std::cout << "connecting to " << args_.host << "\n";

		auto thr = std::thread(
		    [] {
			    std::this_thread::sleep_for(std::chrono::seconds(2));
			    cxx_argp::application::interrupt();
		    });

		std::cout << "waiting for termination request\n";
		cxx_argp::application::wait();
		std::cout << "applicaiton terminates\n";

		thr.join();

		return EXIT_SUCCESS;
	}

public:
	my_app()
	{
		arg_parser.add_option({nullptr,
		                       'h', "host-address", 0,
		                       "hostname or IP-address, only .org addresses allowed"},
		                      args_.host);
	}
};

int main(int argc, char *argv[])
{
	return my_app()(argc, argv);
}
