#include "server.h"

bool kill_threads = false;

void *wrapper_function(void * arg) {
	Server *server = ((Wrapper *)arg)->server;
	server->poll_connections();
	pthread_exit(NULL);
}

Server::Server(std::string port) {
	this->port = port;
	this->backlog = 10;
	this->process_limit = 25;
	this->connection_limit = 2;
	this->connection_count = 0;
	this->id = 0;
	this->max_packet_size = (int)2*(1e4); // 2kB
	this->connections = new connection_info[connection_limit];

	connections[connection_count++] = connection_info(id++, 0, "Server");

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Use host machine's IP

	if ((error = getaddrinfo(NULL, port.c_str(), &hints, &servinfo)) != 0) {
		fprintf(stderr, "addrinfo: %s\n", gai_strerror(error));
		exit(1);
	}
}

void Server::poll_connections() {
	Packet packet = get_default_packet();
	while(true) {

		if (kill_threads) {
			pthread_exit(NULL);
		}

		for (int i=1; i<=connection_count && connections; i++) {
			// Implement polling functions
		}
	}
}

void Server::delete_connection(int connection_id) {
	bool found = false;
	for (int i=1; i<=connection_count; i++) {
		if (!found) {
			if(!connections) {
				break;
			}
			if (connections[i].connection_id == connection_id) {
				close(connections[i].sock_fd);
				found = true;
			}
			continue;
		}
		connections[i-1] = connections[i];
	}
	connection_count--;
}

void Server::delete_all() {
	for (int i=1; i<=connection_count; i++) {
		close(connections[i].sock_fd);
	}
	delete connections;
}

void Server::run() {
	struct sockaddr_storage connecter_address;
	socklen_t sin_size;
	int new_fd;
	pthread_t temp;
	char ip_addr[INET6_ADDRSTRLEN], buf[max_packet_size];

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

	std::cout << "Server running. Awaiting connections!" << std::endl;

	pthread_t poll_connections_thread;
	Wrapper *wrapper = new Wrapper(this);
	pthread_create(&poll_connections_thread, NULL, wrapper_function, (void *)wrapper);

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

        if (connection_count > connection_limit) {
        	std::string str = "Connection limit exceeded\n\0";
        	if (send(new_fd, str.c_str(), str.size(), 0) == -1) {
                perror("send");
        	}
        	close(new_fd);
            continue;
        }

    	std::string str = "Connection accepted. Select username.\n\0";
    	if (send(new_fd, str.c_str(), str.size(), 0) == -1) {
    		perror("send");
    	}

		int stat = recv(new_fd, buf, sizeof buf, 0);
        if (stat == -1 || stat == 0) {
            perror("Server::receive");
            continue;
        }

		std::cout << "\nConnected to " << ip_addr << " as " << buf;

		connections[connection_count++] = connection_info(id++, new_fd, buf);
	}

	kill_threads = true;
	delete_all();
}
