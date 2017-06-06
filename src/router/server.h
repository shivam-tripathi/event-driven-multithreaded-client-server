// server.h
#ifndef SERVER_H
#define SERVER_H

class Server {
	int port;
public:
	Server(int);
	void run();
};

#endif