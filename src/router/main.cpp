#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "server.h"
#include "utils.h"
using namespace std;

int main(int argc, char const *argv[])
{
	cout << "\n\n##################################################################" << endl;
	cout << "# 	   Welcome to hosted end application                     #" << endl;
	cout << "##################################################################" << endl;

	char host_name[20], ip[INET6_ADDRSTRLEN];
	gethostname(host_name, sizeof host_name);
	hostname_to_ip(host_name, ip);

	cout << "You are connected by " << host_name << " on ip address " << ip << endl;

	int port_no;

	cout << "Enter port number for the server to run: "; 
	cin >> port_no; 
	cout << "\nInitializing the server ..." << endl;
	Server server(port_no);
	cout << "Running ... " << endl;
	server.run();

	return 0;
}