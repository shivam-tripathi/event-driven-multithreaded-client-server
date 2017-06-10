#include "server.h"

int main(int argc, char const *argv[])
{
	std::cout << "\n\n##################################################################" << std::endl;
	std::cout << "# 	   Welcome to hosted end application                     #" << std::endl;
	std::cout << "##################################################################" << std::endl;

	char host_name[20], ip[INET6_ADDRSTRLEN];
	gethostname(host_name, sizeof host_name);
	hostname_to_ip(host_name, ip);

	std::cout << "You are connected by " << host_name << " on ip address " << ip << std::endl;

	std::string port;

	std::cout << "Enter port number for the server to run: "; 
	std::cin >> port; 
	std::cout << "\nInitializing the server ..." << std::endl;
	Server server(port);
	std::cout << "Running ... " << std::endl;
	server.run();

	return 0;
}