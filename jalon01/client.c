#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct sockaddr_in sock_host;

int main(int argc,char** argv)
{


    if (argc != 3)
    {
        fprintf(stderr,"usage: RE216_CLIENT hostname port\n");
        return 1;
    }
    do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(& sock_host, '\0', sizeof(sock_host));
    sock_host.sin_family = AF_INET;
    sock_host.sin_port = htons(argv[2]);
    inet_aton(argv[1], & sock_host.sin_addr);
    do_connect(do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP), argv[2], sizeof(argv[2]));
    
    return 0;
}   
   
//get address info from the server
//get_addr_info()





//get the socket
//s = do_socket()
int do_socket(int domain, int type, int protocol) {
	int sockfd;
	int yes = 1;
	//create the socket
	sockfd = socket(domain, type, protocol);

	//check for socket validity

	if (sockfd == -1){
			perror("Socket");
			exit(EXIT_FAILURE);}

	// set socket option, to prevent "already in use" issue when rebooting the server right on

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		error("ERROR setting socket options");

	return sockfd;
}

//connect to remote socket
//do_connect()


void do_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int res = connect(sockfd, addr, addrlen);
	if (res != 0) {
		printf("Unable to find server socket\n");
	}

}



//get user input
//readline()
void readline(){

}


//send message to the server
//handle_client_message()
void handle_client_message(int sockfd){
	int message = 1;
	int *ptr = &message;
	send(sockfd, ptr, 1, 0);
}


