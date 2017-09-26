#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct sockaddr_in sock_host;
struct addrinfo* res;

int main(int argc,char** argv)
{
    if (argc != 3)
    {
        fprintf(stderr,"usage: RE217_CLIENT hostname port\n");
        return 1;
    }

    int sock = do_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&sock_host, '\0', sizeof(sock_host));
    sock_host.sin_family = AF_INET;
    sock_host.sin_port = htons(4000);
    inet_aton("127.0.0.1", & sock_host.sin_addr);
    //printf("%i\n",sock_host.sin_addr);
    get_addr_info("127.0.0.1",4000,&res);
    //do_connect(sock, res->ai_addr, sizeof(res->ai_addr));
    return 0;
}   
   
//get address info from the server
//get_addr_info()

void get_addr_info(const char* address, const char* port, struct addrinfo** res){

	int status;
    struct addrinfo hints, p;

    memset(&hints,0,sizeof(hints));

    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_protocol=0;
    hints.ai_flags=0;

    status = getaddrinfo(address,port,&hints,res);


}

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


int do_connect(int sockfd, struct sockaddr *addr, socklen_t addrlen) {
	int res = connect(sockfd, addr, addrlen);
	if (res != 0) {
		perror("error connection");
		exit(EXIT_FAILURE);
	}
	return 0;
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


