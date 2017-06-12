// server.h
#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

extern bool kill_threads;

class Server : public Pack {
	std::string port;
	int backlog;
	int process_limit;
	int connection_limit;
	int connection_count;
	int sockfd;
	int error;
	int max_packet_size;
	unsigned long long id;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct sigaction signal_action;
	struct connection_info *connections;

	// Remove useless connections
	void delete_connection(int);
	void delete_all();
public:
	Server(std::string);
	void run();
	void poll_connections();
	// May need to implement destructor
	// ~Server();
};

struct Wrapper {
	Server *server;
	Wrapper(Server *server) : server(server) {}
};

void *wrapper_function(void *);

#endif