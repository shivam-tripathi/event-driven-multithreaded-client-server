// server.h
#ifndef SERVER_H
#define SERVER_H

#include "utils.h"

static std::map<pid_t, process_metadata*> running_processes;
static void sigchld_handler(int s);
static int running;

class Server {
	std::string port;
	int backlog;
	int process_limit;
	int connection_limit;
	int connection_count;
	int sockfd;
	int error;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct sigaction signal_action;
	std::vector<std::pair<int, int>> connections;

	// Remove useless connections
	void poll_connection();
public:
	Server(std::string);
	void run();
	// May need to implement destructor
	// ~Server();
};

#endif