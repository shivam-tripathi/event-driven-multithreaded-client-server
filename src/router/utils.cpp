#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "utils.h"

int hostname_to_ip(char * hostname , char* ip) {
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
         
    if ( (he = gethostbyname( hostname ) ) == NULL) {
        // get the host info
        herror("gethostbyname");
        return 1;
    }

    addr_list = (struct in_addr **) he->h_addr_list; 

    for(i = 0; addr_list[i] != NULL; i++) {
        strcpy(ip , inet_ntoa(*addr_list[i]) );
        return 0;
    }
     
    return 1;
}