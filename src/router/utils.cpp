#include "utils.h"

std::set<pid_t> running_processes;

Pack::Pack(int action) {
    packet.action = action;
}

void *sockaddr_to_in(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa) -> sin_addr);
    }
    else {
        return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }
}

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

void sigchld_handler(int s) {
    int saved_errno = errno;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        running_processes.erase(pid);
        if (WEXITSTATUS(status) != 0) {
            std::cout << "Process with id " << pid << " ended unexpectedly." << std::endl;
            // Possible error handling, if needed
        }
    }
    errno = saved_errno;
}
