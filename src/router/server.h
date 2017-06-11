// server.h
#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

class Server {
	std::string port;
	int backlog;
	int process_limit;
	int connection_limit;
	int connection_count;
	int sockfd;
	int error;
	int max_packet_size;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct sigaction signal_action;
	struct connection_info *connections;

	// Remove useless connections
	void poll_connections();
	void delete_connection(int);
public:
	Server(std::string);
	void run();
	// May need to implement destructor
	// ~Server();
};

#endif