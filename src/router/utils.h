// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <map>

// Enum stores various possible actions 
// FTR : File transfer request 
// CJA : Chat join action 
// CPA : Chat post action 

enum Action {
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

// Header packet : Contains header information 
// This is present in all packets
struct header_pack {
	short action;
	size_t msg_size;
	char msg[20];
	header_pack (short x, char *str, int size) {
		action = x; 
		memcpy(msg, str, size);
		msg_size = size;
	}
};

// Stores the data chunks to be transferred
struct data_pack {
	int size;
	char data_chunk[(int)1e4];
	data_pack (char *data, int size) {
		this->size = size;
		memcpy(this->data_chunk, data, size);
	}
};

// Encapsulates the header and data packs into a single packet
// If just a generic packet, the data_pack will point to NULL 
struct Packet {
	header_pack header;
	data_pack data;
	Packet(header_pack header, data_pack data) : header(header), data(data) {}
};

// Class to create and deliver a complete packet
class Pack {
	int pack; 
	Packet packet;
	void get_header_pack();
	void get_data_pack();
public:
	Packet get_packet();
};

struct process_metadata {
	int sockfd;
	char sender_id[20];
	char receiver_id[20];
	sockaddr_storage *sender_saddr;
	sockaddr_storage *receiver_saddr;
};

// Some other generic utility functions
int hostname_to_ip(char *, char*);

void *sockaddr_to_in(struct sockaddr *);

#endif