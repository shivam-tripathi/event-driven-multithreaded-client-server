#include "server.h"


Server::Server(std::string port) {
	this->port = port;
	this->backlog = 10;
	this->process_limit = 25;
	this->connection_limit = 100;
	this->connection_count = 0;
	this->max_packet_size = (int)2*(1e4); // 2kB

	connections.emplace_back(std::tuple<int, int, std::string> (0, 0, "Server"));

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Use host machine's IP

	if ((error = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "addrinfo: %s\n", gai_strerror(error));
		exit(1);
	}
}

void sigchld_handler(int s) {
    int saved_errno = errno;
    int status;
    while(true) {
        pid_t pid = waitpid(-1, &status, WNOHANG);

        if (!pid) return;

        if (WEXITSTATUS(status) != 0) {
            char failed[] = "Failed to send file. Please retry.";
            process_metadata *pmd = running_processes[pid];
            send(pmd->sockfd, failed, sizeof failed, 0);
        }

        running_processes.erase(pid);
    }
}

void Server::run() {
	struct sockaddr_storage connecter_address;
	socklen_t sin_size;
	int new_fd;
	char ip_addr[INET6_ADDRSTRLEN];
	char buf[max_packet_size];

	// Get socket, set socket options and bind it to the socket address
	struct addrinfo *iter;
	int status = true;
	for (iter = servinfo; iter!=NULL; iter = iter->ai_next) {
		if (((this->sockfd) = socket(iter->ai_family, iter->ai_socktype, iter->ai_protocol)) == -1) {
			perror("Server::run : socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &status, sizeof status) == -1) {
			perror("Server::run : setsockopt");
			exit(1);
		} 

		if (bind(sockfd, iter->ai_addr, iter->ai_addrlen) == -1) {
			close(sockfd);
			perror("Server::run : bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo);

	if (iter == NULL) {
		fprintf(stderr, "Server::run : Failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, this->backlog) == -1) {
		perror("Server::run : listen");
		exit(1);
	}

	signal_action.sa_handler = sigchld_handler;
	signal_action.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &signal_action, NULL) == -1) {
		perror("Server::run : sigaction");
		exit(1);
	}

	std::cout << "Server running. Awaiting connections!" << std::endl;

	while (true) {
		sin_size = sizeof connecter_address;

		// A new connection opened
		new_fd = accept(sockfd, (struct sockaddr*) &connecter_address, &sin_size);

		if (new_fd == -1) {
			perror("Server::run : accept");
			continue;
		}

		// Get the socket address of the new connection 
		inet_ntop(connecter_address.ss_family, sockaddr_to_in((struct sockaddr*)&connecter_address), ip_addr, sizeof ip_addr);
		
		int stat = recv(new_fd, buf, sizeof buf, 0);
        if (stat == -1 || stat == 0) {
            perror("Server::receive");
            continue;
        }

		std::cout << "Connected to " << ip_addr << " as " << buf << std::endl;
		connections.emplace_back(std::tuple<int, int, std::string> (++connection_count, new_fd, buf));
	}
}
