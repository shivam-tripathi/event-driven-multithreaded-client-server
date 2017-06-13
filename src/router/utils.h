// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <event.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <utility>
#include <pthread.h>

// using namespace std;

// Enum stores various possible actions
// RTC : Request to connect
// FTR : File transfer request
// CJA : Chat join action
// CPA : Chat post action
// CPA : Connection ping action

enum Action {
	RTC,
	FTR,
	CJA,
	CPA
};

// Pack type type
// COM : Communication generic packet, small in size - only for general purpose message passing
// DATA : Data packet - used in file transfer, of nearly 10 kB size
enum PackType {
	COM,
	DATA
};

// Header part contains header information - action and number of distinct messages
// This is present in all packets
// Packet struct encapsulates the header and data into a single packet
// If just a generic packet, the data will point to NULL
// src_id is the connection id for the packet source
// dest_id is the connection id for the packet destination
// By default src_id and dest_id for the server is 0

struct Packet {
	int action;
	int src_id;
	int dest_id;
	int message_count;
	char *data;
};

// Class to create and deliver a complete packet
class Pack {
	Packet packet;
	void pack_header();
	std::vector<std::string> msg;
public:
	int offset;
	void add_message(std::string);
	void pack_data(char *);
	char *serialize_packet(Packet, size_t);
	Packet deserialize_packet(char *, size_t);
	Packet get_packet();
	Packet get_default_packet();
};

struct connection_info {
	int connection_id;
	int sockfd;
	std::string str;
	struct bufferevent *buf_ev;
	struct evbuffer *output_buffer;

	connection_info() {}
	connection_info(int connection_id, int fd, std::string str) : connection_id(connection_id), sockfd(fd), str(str) {}
};

// Some other generic utility functions

int hostname_to_ip(char *, char*);

void *sockaddr_to_in(struct sockaddr *); // Get sin_addr from the sockaddr depending upon ip version

int setnonblock(int); // Make a socket non blocking

#endif